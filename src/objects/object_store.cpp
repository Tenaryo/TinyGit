#include "objects/object_store.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <zlib.h>

namespace git {

std::string ObjectStore::object_path(std::string_view sha) {
    std::string dir(sha.substr(0, 2));
    std::string file(sha.substr(2));
    return ".git/objects/" + dir + "/" + file;
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

} // namespace git
