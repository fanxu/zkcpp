#include <zookeeper/zookeeper.hh>
#include <iostream>

using namespace org::apache::zookeeper;

int main() {
  ZooKeeper zk;
  std::vector<data::ACL> acl;

  data::ACL perm;
  data::Stat stat;
  perm.getid().getscheme() = "world";
  perm.getid().getid() = "anyone";
  perm.setperms(Permission::All);
  acl.push_back(perm);

  std::string hosts = "ec2-54-254-84-143.ap-southeast-1.compute.amazonaws.com:2181,ec2-175-41-181-250.ap-southeast-1.compute.amazonaws.com:2181,ec2-54-251-217-5.ap-southeast-1.compute.amazonaws.com:2181";
  std::string path = "/search/region/member/172.31.50.193:9001";
  std::string data, pathCreated;
  zk.init(hosts, 30000, boost::shared_ptr<Watch>());
  zk.create(path, "kk", acl, CreateMode::Persistent, pathCreated);
  zk.get(path, boost::shared_ptr<Watch>(), data, stat);
  std::cout << pathCreated << " " << data << std::endl;
//  zk.remove("/hello", -1);
  return 0;
}

