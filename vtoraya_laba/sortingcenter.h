/// @file sortingcenter.h
/// @author Владислав Вертинский
/// @brief Центр хранения и подготовки исходящих почтовых мешков.

#ifndef SORTINGCENTER_H
#define SORTINGCENTER_H

#include "storageinventory.h"
#include "mailbag.h"
#include <cstddef>
#include <vector>

/// @brief Центр хранения и подготовки исходящих почтовых мешков.
/// @details Сортировка переносит запись из ячейки в мешок. Выдача готового мешка удаляет его из центра.
class SortingCenter {
private:
    int id_; ///< Идентификатор объекта.
    int branchId_; ///< Идентификатор отделения.
    StorageInventory inventory_; ///< Складской учёт центра.
    std::vector<MailBag> bags_; ///< Мешки центра или транспорта.
    bool open_; ///< Открыта ли смена или окно.

    /// @brief Проверяет, что смена объекта открыта.
    void ensureOpen() const;
    /// @brief Находит исходящий мешок для внутреннего изменения.
    /// @param bagId Идентификатор мешка.
    /// @return Находит исходящий мешок для внутреннего изменения.
    MailBag &findMutableBag(const std::string &bagId);

public:
    /// @brief Создаёт закрытый сортировочный центр без зон и мешков.
    /// @param id Идентификатор записи.
    /// @param branch Отделение, с которым связана операция.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    SortingCenter(int id, const PostalBranch &branch, int maximumWeight);

    /// @brief Идентификатор объекта.
    /// @return Идентификатор объекта.
    int id() const;
    /// @brief Идентификатор отделения.
    /// @return Идентификатор отделения.
    int branchId() const;
    /// @brief Признак открытой смены или окна.
    /// @return Признак открытой смены или окна.
    bool isOpen() const;
    /// @brief Число отправлений, ещё находящихся в ячейках центра.
    /// @return Число отправлений, ещё находящихся в ячейках центра.
    std::size_t storedCount() const;
    /// @brief Число учитываемых мешков.
    /// @return Число учитываемых мешков.
    std::size_t bagCount() const;
    /// @brief Получает зафиксированный вес хранящегося отправления.
    /// @return Сохранённый вес отправления в граммах.
    long long storedWeightGrams() const;
    /// @brief Добавляет копию пустой зоны своего отделения.
    /// @param zone Пустая зона нужного отделения.
    void addZone(const StorageZone &zone);
    /// @brief Открывает смену объекта.
    void openShift();
    /// @brief Закрывает открытую смену объекта.
    void closeShift();
    /// @brief Принимает отправление на хранение, исключая повторный приём уже рассортированного вложения.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param contents Габариты вложения; при подборе допускается перестановка осей.
    /// @return Адрес занятой ячейки.
    StorageLocation receiveShipment(const Shipment &shipment, const Dimensions &contents);
    /// @brief Находит адрес отправления, пока оно находится в ячейке центра.
    /// @param trackingNumber Трек-номер без пробельных символов.
    /// @return Место учёта или хранения найденного отправления.
    StorageLocation locateShipment(const std::string &trackingNumber) const;
    /// @brief Создаёт пустой мешок для другого отделения.
    /// @param bagId Идентификатор мешка.
    /// @param destinationBranchId Положительный ID другого отделения назначения.
    /// @param maximumWeight Положительный предел веса вложений в граммах.
    void createBag(const std::string &bagId, int destinationBranchId, int maximumWeight);
    /// @brief Находит исходящий мешок центра по ID.
    /// @param bagId Идентификатор мешка.
    /// @return Ссылка на исходящий мешок центра.
    /// @note Создание или выдача мешка может сделать ранее полученную ссылку недействительной.
    const MailBag &findBag(const std::string &bagId) const;
    /// @brief Переносит запись из ячейки в мешок после проверки веса и доступности мешка.
    /// @param shipment Отправление, над которым выполняется операция.
    /// @param bagId Идентификатор мешка.
    void sortIntoBag(const Shipment &shipment, const std::string &bagId);
    /// @brief Запечатывает заполненный исходящий мешок.
    /// @param bagId Идентификатор мешка.
    void sealBag(const std::string &bagId);
    /// @brief Выдаёт копию запечатанного мешка и удаляет его из центра.
    /// @param bagId Идентификатор мешка.
    /// @return Самостоятельная копия выданного мешка.
    MailBag takeBag(const std::string &bagId);
};

#endif
