/*
 * Project: curve
 * File Created: 2019-12-03
 * Author: charisu
 * Copyright (c)￼ 2018 netease
 */

#ifndef SRC_TOOLS_STATUS_TOOL_H_
#define SRC_TOOLS_STATUS_TOOL_H_

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <brpc/channel.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "proto/topology.pb.h"
#include "src/common/timeutility.h"
#include "src/mds/common/mds_define.h"
#include "src/tools/mds_client.h"
#include "src/tools/chunkserver_client.h"
#include "src/tools/namespace_tool_core.h"
#include "src/tools/copyset_check_core.h"
#include "src/tools/etcd_client.h"
#include "src/tools/version_tool.h"
#include "src/tools/curve_tool.h"
#include "src/tools/curve_tool_define.h"
#include "src/tools/metric_client.h"
#include "src/tools/metric_name.h"
#include "src/tools/snapshot_clone_client.h"

using curve::mds::topology::ChunkServerStatus;
using curve::mds::topology::DiskState;
using curve::mds::topology::OnlineState;
using curve::mds::topology::PhysicalPoolInfo;
using curve::mds::topology::LogicalPoolInfo;
using curve::mds::topology::PoolIdType;
using curve::mds::topology::ChunkServerInfo;

namespace curve {
namespace tool {
struct SpaceInfo {
    uint64_t total = 0;
    uint64_t logicalUsed = 0;
    uint64_t physicalUsed = 0;
    uint64_t canBeRecycled = 0;
};

enum class ServiceName {
    // mds
    kMds,
    kEtcd,
    kSnapshotCloneServer,
};

std::string ToString(ServiceName name);

class StatusTool : public CurveTool {
 public:
    StatusTool(std::shared_ptr<MDSClient> mdsClient,
               std::shared_ptr<EtcdClient> etcdClient,
               std::shared_ptr<NameSpaceToolCore> nameSpaceToolCore,
               std::shared_ptr<CopysetCheckCore> copysetCheckCore,
               std::shared_ptr<VersionTool> versionTool,
               std::shared_ptr<MetricClient> metricClient,
               std::shared_ptr<SnapshotCloneClient> snapshotClient) :
                  mdsClient_(mdsClient), etcdClient_(etcdClient),
                  nameSpaceToolCore_(nameSpaceToolCore),
                  copysetCheckCore_(copysetCheckCore),
                  versionTool_(versionTool),
                  metricClient_(metricClient),
                  snapshotClient_(snapshotClient),
                  mdsInited_(false), etcdInited_(false) {}
    ~StatusTool() = default;

    /**
     *  @brief 打印help信息
     *  @param cmd：执行的命令
     *  @return 无
     */
    void PrintHelp(const std::string &command) override;

    /**
     *  @brief 执行命令
     *  @param cmd：执行的命令
     *  @return 成功返回0，失败返回-1
     */
    int RunCommand(const std::string &command) override;

    /**
     *  @brief 返回是否支持该命令
     *  @param command：执行的命令
     *  @return true / false
     */
    static bool SupportCommand(const std::string& command);

    /**
     *  @brief 判断集群是否健康
     */
    bool IsClusterHeatlhy();

 private:
    int Init(const std::string& command);
    int SpaceCmd();
    int StatusCmd();
    int ChunkServerListCmd();
    int ChunkServerStatusCmd();
    int GetPoolsInCluster(std::vector<PhysicalPoolInfo>* phyPools,
                          std::vector<LogicalPoolInfo>* lgPools);
    int GetSpaceInfo(SpaceInfo* spaceInfo);
    int PrintClusterStatus();
    int PrintMdsStatus();
    int PrintEtcdStatus();
    int PrintChunkserverStatus(bool checkLeftSize = true);
    int PrintClientStatus();
    int ClientListCmd();
    void PrintCsLeftSizeStatistics(const std::vector<uint64_t>& leftSize);
    int PrintSnapshotCloneStatus();

    /**
     *  @brief 判断命令是否需要和etcd交互
     *  @param command：执行的命令
     *  @return 需要返回true，否则返回false
     */
    bool CommandNeedEtcd(const std::string& command);


    /**
     *  @brief 判断命令是否需要mds
     *  @param command：执行的命令
     *  @return 需要返回true，否则返回false
     */
    bool CommandNeedMds(const std::string& command);

    /**
     *  @brief 判断命令是否需要snapshot clone server
     *  @param command：执行的命令
     *  @return 需要返回true，否则返回false
     */
    bool CommandNeedSnapshotClone(const std::string& command);

    /**
     *  @brief 打印在线状态
     *  @param name : 在线状态对应的名字
     *  @param onlineStatus 在线状态的map
     */
    void PrintOnlineStatus(const std::string& name,
                           const std::map<std::string, bool>& onlineStatus);

    /**
     *  @brief 获取并打印mds version信息
     */
    int GetAndPrintMdsVersion();

    /**
     *  @brief 检查服务是否健康
     *  @param name 服务名
     */
    bool CheckServiceHealthy(const ServiceName& name);

 private:
    // 向mds发送RPC的client
    std::shared_ptr<MDSClient> mdsClient_;
    // NameSpace工具，用于获取recycleBin的大小
    std::shared_ptr<NameSpaceToolCore> nameSpaceToolCore_;
    // Copyset检查工具，用于检查集群和chunkserver的健康状态
    std::shared_ptr<CopysetCheckCore> copysetCheckCore_;
    // etcd client，用于调etcd API获取状态
    std::shared_ptr<EtcdClient> etcdClient_;
    // 用于获取metric
    std::shared_ptr<MetricClient> metricClient_;
    // 用于获取snapshot clone的状态
    std::shared_ptr<SnapshotCloneClient> snapshotClient_;
    // version client，用于获取version信息
    std::shared_ptr<VersionTool> versionTool_;
    // mds是否初始化过
    bool mdsInited_;
    // etcd是否初始化过
    bool etcdInited_;
};
}  // namespace tool
}  // namespace curve

#endif  // SRC_TOOLS_STATUS_TOOL_H_
