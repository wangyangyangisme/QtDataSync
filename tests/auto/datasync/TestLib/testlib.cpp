#include "testlib.h"
using namespace QtDataSync;

const QByteArray TestLib::TypeName("TestData");
QTemporaryDir TestLib::tDir;

void TestLib::init()
{
	qRegisterMetaType<TestData>();
	QJsonSerializer::registerListConverters<TestData>();
}

Setup &TestLib::setup(Setup &setup)
{
	return setup.setLocalDir(tDir.path());
}

ObjectKey TestLib::generateKey(int index)
{
	return {TypeName, QString::number(index)};
}

TestData TestLib::generateData(int index)
{
	return {index, QString::number(index)};
}

QList<TestData> TestLib::generateData(int from, int to)
{
	QList<TestData> list;
	for(auto i = from; i <= to; i++)
		list.append({i, QString::number(i)});
	return list;
}

QStringList TestLib::generateDataKeys(int from, int to)
{
	QStringList list;
	for(auto i = from; i <= to; i++)
		list.append(QString::number(i));
	return list;
}

QJsonObject TestLib::generateDataJson(int index)
{
	QJsonObject data;
	data[QStringLiteral("id")] = index;
	data[QStringLiteral("text")] = QString::number(index);
	return data;
}

TestLib::DataSet TestLib::generateDataJson(int from, int to)
{
	DataSet hash;
	for(auto i = from; i <= to; i++)
		hash.insert(generateKey(i), generateDataJson(i));
	return hash;
}

QJsonArray TestLib::dataListJson(const TestLib::DataSet &data)
{
	QJsonArray v;
	foreach(auto d, data)
		v.append(d);
	return v;
}