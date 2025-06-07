#include "services/SalesService.h"
#include <algorithm>
#include <numeric>

// --- Statistics Services ---
DailyStatistics SalesService::getStatisticsForDate(const std::string& date) const {
    DailyStatistics stats; // Tạo một đối tượng struct để chứa kết quả

    for (const auto& inv : invoices) {
        // So sánh 10 ký tự đầu (dd/mm/yyyy)
        if (inv.getDate().rfind(date, 0) == 0) {
            stats.invoiceCount++; // Tăng số đếm hóa đơn
            stats.totalRevenue += inv.getTotalAmount(); // Cộng dồn doanh thu
        }
    }
    return stats; 
}

std::map<int, DailyStatistics> SalesService::getDailyStatisticsForMonth(int month, int year) const {
    std::map<int, DailyStatistics> dailyStats; // Kiểu trả về mới

    for (const auto& inv : invoices) {
        int invDay, invMonth, invYear;
        if (sscanf(inv.getDate().c_str(), "%d/%d/%d", &invDay, &invMonth, &invYear) == 3) {
            if (invMonth == month && invYear == year) {
                // Truy cập vào struct DailyStatistics của ngày tương ứng
                dailyStats[invDay].invoiceCount++; 
                dailyStats[invDay].totalRevenue += inv.getTotalAmount();
            }
        }
    }
    return dailyStats; 
}

std::map<int, MonthlyStatistics> SalesService::getMonthlyStatisticsForYear(int year) const {
    std::map<int, MonthlyStatistics> monthlyStats; // Kiểu trả về mới

    for (const auto& inv : invoices) {
        int invDay, invMonth, invYear;
        if (sscanf(inv.getDate().c_str(), "%d/%d/%d", &invDay, &invMonth, &invYear) == 3) {
            if (invYear == year) {
                // Truy cập vào struct MonthlyStatistics của tháng tương ứng
                monthlyStats[invMonth].invoiceCount++;
                monthlyStats[invMonth].totalRevenue += inv.getTotalAmount();
            }
        }
    }
    return monthlyStats; 
}

std::vector<ProductRevenue> SalesService::getProductRevenueByMonth(int month, int year) const {
    std::map<std::string, ProductRevenue> productStats;
    for (const auto& inv : invoices) {
        int invDay, invMonth, invYear;
        if (sscanf(inv.getDate().c_str(), "%d/%d/%d", &invDay, &invMonth, &invYear) == 3) {
            if (invMonth == month && invYear == year) {
                for (const auto& item : inv.getItems()) {
                    const auto& p = item.getProduct();
                    auto& stat = productStats[p.getProductId()];
                    if (stat.id.empty()){
                        stat.id = p.getProductId();
                        stat.name = p.getProductName();
                        stat.priceAtTime = p.getPrice();
                        stat.stillExists = this->findProductById(p.getProductId()) != nullptr;
                    }
                    stat.totalQuantity += item.getQuantity();
                    stat.totalRevenue += item.getTotalPrice();
                }
            }
        }
    }
    std::vector<ProductRevenue> result;
    for(const auto& pair : productStats) { result.push_back(pair.second); }
    
    std::sort(result.begin(), result.end(), [](const ProductRevenue& a, const ProductRevenue& b){
        if (a.stillExists != b.stillExists) return a.stillExists > b.stillExists;
        return a.totalQuantity > b.totalQuantity;
    });
    
    return result;
}