#include "core/object_store.hpp"
#include "util/hex.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <openssl/sha.h>
#include <sstream>
#include <zlib.h>

namespace core {

auto ObjectStore::store_object(const std::string& raw_data)
    -> std::expected<std::string, std::string> {
    std::string sha = compute_sha1(raw_data);
    auto compressed = compress(raw_data);
    if (!compressed) {
        return std::unexpected(compressed.error());
    }
    auto result = write_object(sha, *compressed);
    if (!result) {
        return std::unexpected(result.error());
    }
    return sha;
}

auto ObjectStore::object_path(std::string_view sha) -> std::string {
    std::string dir(sha.substr(0, 2));
    std::string file(sha.substr(2));
    return ".git/objects/" + dir + "/" + file;
}

auto ObjectStore::compress(const std::string& data) -> std::expected<std::string, std::string> {
    z_stream stream{};
    stream.next_in =
        reinterpret_cast<Bytef*>(const_cast<char*>(data.data())); // zlib never modifies input
    stream.avail_in = static_cast<uInt>(data.size());

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return std::unexpected("Failed to initialize zlib for compression");
    }

    std::string compressed;
    std::array<char, 4096> buffer{};

    while (true) {
        stream.next_out = reinterpret_cast<Bytef*>(buffer.data());
        stream.avail_out = static_cast<uInt>(buffer.size());

        int ret = deflate(&stream, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&stream);
            return std::unexpected("Zlib compression stream error");
        }

        compressed.append(buffer.data(), buffer.size() - stream.avail_out);

        if (ret == Z_STREAM_END) {
            break;
        }
    }

    deflateEnd(&stream);
    return compressed;
}

auto ObjectStore::decompress(const std::string& compressed)
    -> std::expected<std::string, std::string> {
    z_stream stream{};
    stream.next_in =
        reinterpret_cast<Bytef*>(const_cast<char*>(compressed.data())); // zlib never modifies input
    stream.avail_in = static_cast<uInt>(compressed.size());

    if (inflateInit(&stream) != Z_OK) {
        return std::unexpected("Failed to initialize zlib for decompression");
    }

    std::string decompressed;
    std::array<char, 4096> buffer{};

    while (true) {
        stream.next_out = reinterpret_cast<Bytef*>(buffer.data());
        stream.avail_out = static_cast<uInt>(buffer.size());

        int ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&stream);
            return std::unexpected("Failed to decompress data");
        }

        decompressed.append(buffer.data(), buffer.size() - stream.avail_out);

        if (ret == Z_STREAM_END) {
            break;
        }
    }

    inflateEnd(&stream);
    return decompressed;
}

auto ObjectStore::compute_sha1(const std::string& data) -> std::string {
    std::array<unsigned char, SHA_DIGEST_LENGTH> hash{};
    SHA1(reinterpret_cast<const unsigned char*>(data.data()), data.size(), hash.data());
    return util::bytes_to_hex(hash);
}

auto ObjectStore::read_object(std::string_view sha) -> std::expected<std::string, std::string> {
    std::string path = object_path(sha);

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::unexpected("Failed to read object file: " + path);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    std::string compressed = stream.str();

    return decompress(compressed);
}

auto ObjectStore::write_object(std::string_view sha, const std::string& compressed_data)
    -> std::expected<void, std::string> {
    std::string path = object_path(sha);
    std::string dir = ".git/objects/" + std::string(sha.substr(0, 2));

    std::error_code error_code;
    if (!std::filesystem::exists(dir)) {
        if (!std::filesystem::create_directory(dir, error_code)) {
            return std::unexpected("Failed to create directory: " + dir);
        }
    }

    std::ofstream file(path, std::ios::binary);
    if (!file) {
        return std::unexpected("Failed to open file for writing: " + path);
    }

    file.write(compressed_data.data(), static_cast<std::streamsize>(compressed_data.size()));
    if (!file) {
        return std::unexpected("Failed to write object data");
    }

    return {};
}

} // namespace core
