#include "objects/object_store.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <openssl/sha.h>
#include <sstream>
#include <zlib.h>

namespace git {

std::string ObjectStore::object_path(std::string_view sha) {
    std::string dir(sha.substr(0, 2));
    std::string file(sha.substr(2));
    return ".git/objects/" + dir + "/" + file;
}

std::string ObjectStore::compress(const std::string& data) {
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(data.data()));
    stream.avail_in = static_cast<uInt>(data.size());

    if (deflateInit(&stream, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return "";
    }

    std::string compressed;
    char buffer[4096];

    while (true) {
        stream.next_out = reinterpret_cast<Bytef*>(buffer);
        stream.avail_out = sizeof(buffer);

        int ret = deflate(&stream, Z_FINISH);
        if (ret == Z_STREAM_ERROR) {
            deflateEnd(&stream);
            return "";
        }

        compressed.append(buffer, sizeof(buffer) - stream.avail_out);

        if (ret == Z_STREAM_END) {
            break;
        }
    }

    deflateEnd(&stream);
    return compressed;
}

std::expected<std::string, std::string> ObjectStore::decompress(const std::string& compressed) {
    z_stream stream{};
    stream.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(compressed.data()));
    stream.avail_in = static_cast<uInt>(compressed.size());

    if (inflateInit(&stream) != Z_OK) {
        return std::unexpected("Failed to initialize zlib");
    }

    std::string decompressed;
    char buffer[4096];

    while (true) {
        stream.next_out = reinterpret_cast<Bytef*>(buffer);
        stream.avail_out = sizeof(buffer);

        int ret = inflate(&stream, Z_NO_FLUSH);
        if (ret == Z_STREAM_ERROR || ret == Z_DATA_ERROR || ret == Z_MEM_ERROR) {
            inflateEnd(&stream);
            return std::unexpected("Failed to decompress data");
        }

        decompressed.append(buffer, sizeof(buffer) - stream.avail_out);

        if (ret == Z_STREAM_END) {
            break;
        }
    }

    inflateEnd(&stream);
    return decompressed;
}

std::string ObjectStore::compute_sha1(const std::string& data) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.data()), data.size(), hash);

    std::ostringstream oss;
    for (unsigned char c : hash) {
        oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
    }
    return oss.str();
}

std::expected<std::string, std::string> ObjectStore::read_object(std::string_view sha) {
    std::string path = object_path(sha);

    if (!std::filesystem::exists(path)) {
        return std::unexpected("Object not found: " + path);
    }

    std::ifstream file(path, std::ios::binary);
    if (!file) {
        return std::unexpected("Failed to open object file: " + path);
    }

    std::stringstream ss;
    ss << file.rdbuf();
    std::string compressed = ss.str();

    return decompress(compressed);
}

std::expected<void, std::string> ObjectStore::write_object(std::string_view sha,
                                                           const std::string& compressed_data) {
    std::string path = object_path(sha);
    std::string dir = ".git/objects/" + std::string(sha.substr(0, 2));

    std::error_code ec;
    if (!std::filesystem::exists(dir)) {
        if (!std::filesystem::create_directory(dir, ec)) {
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

} // namespace git
