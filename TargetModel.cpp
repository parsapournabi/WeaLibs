//#include "TargetModel.h"

//TargetModel::TargetModel(QObject *parent)
//    : QAbstractListModel(parent)
//{
//}

//int TargetModel::rowCount(const QModelIndex &) const {
//    return m_targets.size();
//}

//QVariant TargetModel::data(const QModelIndex &index, int role) const {
//    if (!index.isValid())
//        return {};

//    Target *t = m_targets.at(index.row());

//    switch (role) {
//    case AzimuthRole: return t->azimuth();
//    case ElevationRole: return t->elevation();
//    case RangeCellRole: return t->rangeCell();
//    case PowerRole: return t->power();
//    case SelectedRole: return t->selected();
//    case RowRole: return t->row();
//    case TargetObjectRole: return QVariant::fromValue(t);

//    default: return {};
//    }
//}

//QHash<int, QByteArray> TargetModel::roleNames() const {
//    return {
//        {AzimuthRole, "azimuth"},
//        {ElevationRole, "elevation"},
//        {RangeCellRole, "rangeCell"},
//        {PowerRole, "power"},
//        { SelectedRole, "selected" },
//        { RowRole, "row" },
//        {TargetObjectRole, "targetObject"}
//    };
//}

//void TargetModel::addTarget(Target *target) {
//    beginInsertRows(QModelIndex(), m_targets.size(), m_targets.size());
//    m_targets.append(target);
//    connect(target, &Target::dataChanged, this, [=]() {
//        int row = m_targets.indexOf(target);
//        if (row >= 0)
//            emit dataChanged(index(row), index(row));
//    });
//    target->setRow(lastRow);
//    lastRow++;
//    qDebug() << target->row();
//    endInsertRows();
//}

//Target* TargetModel::getTarget(int row) const {
//    return m_targets.value(row);
//}

//Q_INVOKABLE void TargetModel::addTargetFromQml(float az, float el, float range, float power) {
//    Target* t = new Target();
//    t->setAzimuth(az);
//    t->setElevation(el);
//    t->setRangeCell(range);
//    t->setPower(power);
//    addTarget(t);

//}


//Q_INVOKABLE void TargetModel::removeLast() {
//    if (m_targets.isEmpty()) return;
//    beginRemoveRows({}, m_targets.size() - 1, m_targets.size() - 1);
//    delete m_targets.takeLast();
//    lastRow--;
//    endRemoveRows();
//}

//Q_INVOKABLE void TargetModel::selectTarget(int index) {
//    qDebug() << "Select row: " << index;
//    for (int i = 0; i < m_targets.size(); ++i)
//        m_targets[i]->setSelected(i == index);
//    qDebug() << m_targets[index]->selected();
//    emit dataChanged(this->index(0), this->index(m_targets.size() - 1));
//}

