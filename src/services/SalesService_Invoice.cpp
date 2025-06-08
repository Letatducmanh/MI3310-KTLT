#include "services/SalesService.h"
#include <algorithm>

// --- Invoice Services ---
const std::vector<Invoice>& SalesService::getAllInvoices() const { return invoices; }

Invoice* SalesService::findInvoiceById(const std::string& id) {
    auto it = std::find_if(invoices.begin(), invoices.end(), [&](const Invoice& inv){ return inv.getInvoiceId() == id; });
    return (it != invoices.end()) ? &(*it) : nullptr;
}

const Invoice* SalesService::findInvoiceById(const std::string& id) const {
    auto it = std::find_if(invoices.begin(), invoices.end(), [&](const Invoice& inv){ return inv.getInvoiceId() == id; });
    return (it != invoices.end()) ? &(*it) : nullptr;
}

bool SalesService::isInvoiceIdTaken(const std::string& id) const {
    return findInvoiceById(id) != nullptr;
}

void SalesService::createInvoice(const Invoice& newInvoice) {
    invoices.push_back(newInvoice);
    dataManager.appendInvoiceToLog(newInvoice);
}