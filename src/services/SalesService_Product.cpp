#include "services/SalesService.h"
#include <algorithm>

// --- Product Services ---
const std::vector<Product>& SalesService::getAllProducts() const { return products; }

Product* SalesService::findProductById(const std::string& id) {
    auto it = std::find_if(products.begin(), products.end(), [&](const Product& p){ return p.getProductId() == id; });
    return (it != products.end()) ? &(*it) : nullptr;
}

const Product* SalesService::findProductById(const std::string& id) const {
    auto it = std::find_if(products.begin(), products.end(), [&](const Product& p){ return p.getProductId() == id; });
    return (it != products.end()) ? &(*it) : nullptr;
}

bool SalesService::isProductIdTaken(const std::string& id) const {
    return findProductById(id) != nullptr;
}

void SalesService::addProduct(const Product& newProduct) {
    products.push_back(newProduct);
}

bool SalesService::deleteProduct(const std::string& id) {
    auto it = std::remove_if(products.begin(), products.end(), [&](const Product& p) { return p.getProductId() == id; });
    if (it != products.end()) {
        products.erase(it, products.end());
        return true;
    }
    return false;
}

ImportResult SalesService::importProductsFromFile(const std::string& filename) {
    auto resultPair = dataManager.loadProductsFromImportFile(filename, this->products);
    const auto& newProducts = resultPair.first;
    if (!newProducts.empty()) {
        products.insert(products.end(), newProducts.begin(), newProducts.end());
    }
    return resultPair.second;
}