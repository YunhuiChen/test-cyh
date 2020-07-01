/*
 * Project: curve
 * File Created: 18-10-31
 * Author: yangyaokai
 * Copyright (c) 2018 netease
 */

#ifndef SRC_FS_FS_COMMON_H_
#define SRC_FS_FS_COMMON_H_

namespace curve {
namespace fs {

enum class FileSystemType {
    // SFS,
    EXT4,
};

struct FileSystemInfo {
    uint64_t total = 0;         // Total bytes
    uint64_t available = 0;     // Free bytes available for unprivileged users
    uint64_t allocated = 0;     // Bytes allocated by the store
    uint64_t stored = 0;        // Bytes actually stored by the user
};

}  // namespace fs
}  // namespace curve
#endif  // SRC_FS_FS_COMMON_H_
