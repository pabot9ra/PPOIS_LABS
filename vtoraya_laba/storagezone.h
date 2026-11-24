/// @file storagezone.h
/// @author Владислав Вертинский
/// @brief Зона с ячейками и блокировкой размещения и выдачи.

#ifndef STORAGEZONE_H
#define STORAGEZONE_H

#include "storagecell.h"
#include <cstddef>
#include <vector>

/// @brief Зона с ячейками и блокировкой размещения и выдачи.
/// @details Хранит собственные копии ячеек. Блокировка не удаляет сведения о содержимом.
class StorageZone {
private:
    std::string id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    std::vector<StorageCell> cells_; ///< Копии ячеек зоны.
    bool locked_; ///< Заблокирована ли зона.

public:
    /// @brief Создаёт пустую разблокированную зону.
    /// @param id Идентификатор записи.
    /// @param branchId Идентификатор отделения.
    StorageZone(const std::string &id, int branchId);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    const std::string &id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Признак заблокированной зоны.
    /// @return Признак заблокированной зоны.
    bool locked() const;
    /// @brief Число ячеек зоны.
    /// @return Число ячеек зоны.
    std::size_t cellCount() const;
    /// @brief Число занятых ячеек зоны.
    /// @return Число занятых ячеек зоны.
    std::size_t occupiedCount() const;
    /// @brief Масса содержимого в граммах.
    /// @return Масса содержимого в граммах.
    long long weightGrams() const;
    /// @brief Добавляет копию пустой ячейки своего отделения с уникальным ID.
    /// @param cell Пустая ячейка нужного отделения.
    void addCell(const StorageCell &cell);
    /// @brief Находит ячейку зоны по ID.
    /// @param id Идентификатор записи.
    /// @return Ссылка на ячейку зоны.
    /// @note Добавление новой ячейки может сделать ранее полученную ссылку недействительной.
    const StorageCell &findCell(const std::string &id) const;
    /// @brief Находит занятую ячейку по трек-номеру.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Ссылка на ячейку с указанным отправлением.
    const StorageCell &findShipmentCell(const std::string &trackingNumber) const;
    /// @brief Находит свободную подходящую ячейку в разблокированной зоне.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    /// @return Ссылка на первую подходящую свободную ячейку.
    const StorageCell &findAvailableCell(const Shipment &shipment, const Dimensions &contents) const;
    /// @brief Проверяет наличие записи по идентификатору.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return true, если запись найдена; иначе false.
    bool contains(const std::string &trackingNumber) const;
    /// @brief Проверяет наличие подходящей ячейки и отсутствие повторной записи.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    /// @return true, если размещение допустимо и найдена подходящая ячейка; иначе false.
    bool canStore(const Shipment &shipment, const Dimensions &contents) const;
    /// @brief Размещает отправление в первой подходящей свободной ячейке.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    void storeShipment(const Shipment &shipment, const Dimensions &contents);
    /// @brief Удаляет запись отправления из хранилища, освобождая ячейку.
    /// @param trackingNumber Трек-номер без пробельных символов.
    void releaseShipment(const std::string &trackingNumber);
    /// @brief Блокирует размещение и выдачу в зоне.
    void lock();
    /// @brief Снимает блокировку зоны.
    void unlock();
};

#endif
