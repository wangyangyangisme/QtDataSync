#ifndef ASYNCDATASTORE_H
#define ASYNCDATASTORE_H

#include "QtDataSync/qdatasync_global.h"
#include "QtDataSync/task.h"

#include <QtCore/qobject.h>
#include <QtCore/qfuture.h>
#include <QtCore/qmetaobject.h>

namespace QtDataSync {

class Setup;

class AsyncDataStorePrivate;
class Q_DATASYNC_EXPORT AsyncDataStore : public QObject
{
	Q_OBJECT

public:
	explicit AsyncDataStore(QObject *parent = nullptr);
	explicit AsyncDataStore(const QString &setupName, QObject *parent = nullptr);
	~AsyncDataStore();

	GenericTask<int> count(int metaTypeId);
	GenericTask<QStringList> keys(int metaTypeId);
	Task loadAll(int dataMetaTypeId, int listMetaTypeId);
	Task load(int metaTypeId, const QString &key);
	Task load(int metaTypeId, const QVariant &key);
	Task save(int metaTypeId, const QVariant &value);
	Task remove(int metaTypeId, const QString &key);
	Task remove(int metaTypeId, const QVariant &key);
	Task search(int dataMetaTypeId, int listMetaTypeId, const QString &query);

	//WARNING -> DOES NOT TAKE OWNERSHIP OF QOBJECTS
	template<typename T>
	GenericTask<int> count();
	template<typename T>
	GenericTask<QStringList> keys();
	template<typename T>
	GenericTask<QList<T>> loadAll();
	template<typename T>
	GenericTask<T> load(const QString &key);
	template<typename T, typename K>
	GenericTask<T> load(const K &key);
	template<typename T>
	GenericTask<void> save(const T &value);
	template<typename T>
	GenericTask<bool> remove(const QString &key);
	template<typename T, typename K>
	GenericTask<bool> remove(const K &key);
	template<typename T>
	GenericTask<QList<T>> search(const QString &query);

Q_SIGNALS:
	void dataChanged(int metaTypeId, const QString &key, bool wasDeleted);
	void dataResetted();

private:
	QScopedPointer<AsyncDataStorePrivate> d;

	QFutureInterface<QVariant> internalCount(int metaTypeId);
	QFutureInterface<QVariant> internalKeys(int metaTypeId);
	QFutureInterface<QVariant> internalLoadAll(int dataMetaTypeId, int listMetaTypeId);
	QFutureInterface<QVariant> internalLoad(int metaTypeId, const QString &key);
	QFutureInterface<QVariant> internalSave(int metaTypeId, const QVariant &value);
	QFutureInterface<QVariant> internalRemove(int metaTypeId, const QString &key);
	QFutureInterface<QVariant> internalSearch(int dataMetaTypeId, int listMetaTypeId, const QString &query);
};

template<typename T>
GenericTask<int> AsyncDataStore::count()
{
	return internalCount(qMetaTypeId<T>());
}

template<typename T>
GenericTask<QStringList> AsyncDataStore::keys()
{
	return internalKeys(qMetaTypeId<T>());
}

template<typename T>
GenericTask<QList<T>> AsyncDataStore::loadAll()
{
	return internalLoadAll(qMetaTypeId<T>(), qMetaTypeId<QList<T>>());
}

template<typename T>
GenericTask<T> AsyncDataStore::load(const QString &key)
{
	return internalLoad(qMetaTypeId<T>(), key);
}

template<typename T, typename K>
GenericTask<T> AsyncDataStore::load(const K &key)
{
	return internalLoad(qMetaTypeId<T>(), QVariant::fromValue(key).toString());
}

template<typename T>
GenericTask<void> AsyncDataStore::save(const T &value)
{
	return internalSave(qMetaTypeId<T>(), QVariant::fromValue(value));
}

template<typename T>
GenericTask<bool> AsyncDataStore::remove(const QString &key)
{
	return internalRemove(qMetaTypeId<T>(), key);
}

template<typename T, typename K>
GenericTask<bool> AsyncDataStore::remove(const K &key)
{
	return internalRemove(qMetaTypeId<T>(), QVariant::fromValue(key).toString());
}

template<typename T>
GenericTask<QList<T>> AsyncDataStore::search(const QString &query)
{
	return internalSearch(qMetaTypeId<T>(), qMetaTypeId<QList<T>>(), query);
}

}

#endif // ASYNCDATASTORE_H