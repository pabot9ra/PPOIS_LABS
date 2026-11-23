#ifndef REQUESTREGISTRY_H
#define REQUESTREGISTRY_H

#include "customerrequest.h"
#include "postalbranch.h"
#include <cstddef>
#include <memory>
#include <vector>

class RequestRegistry {
private:
    int branchId_;
    std::vector<std::unique_ptr<CustomerRequest>> requests_;

    void ensureBranch(const PostalBranch &branch) const;
    CustomerRequest &findMutableRequest(int id);

public:
    explicit RequestRegistry(const PostalBranch &branch);

    int branchId() const;
    std::size_t requestCount() const;
    void submit(const PostalBranch &branch, const CustomerRequest &request);
    const CustomerRequest &findRequest(int id) const;
    void approve(int id, const PostalBranch &branch);
    void reject(int id, const std::string &reason);
    std::string issueReturnLabel(int id, const PostalBranch &branch);
    std::string issueRedirectionLabel(int id, const PostalBranch &branch);
    void settleDamageClaim(int id, PostalBranch &branch);
};

#endif
