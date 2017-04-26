#include "synccontroller.h"
#include "synccontroller_p.h"
#include "setup_p.h"

using namespace QtDataSync;

SyncController::SyncController(QObject *parent) :
	SyncController(Setup::DefaultSetup, parent)
{}

SyncController::SyncController(const QString &setupName, QObject *parent) :
	QObject(parent),
	d(new SyncControllerPrivate())
{
	d->engine = SetupPrivate::engine(setupName);
	Q_ASSERT_X(d->engine, Q_FUNC_INFO, "SyncController requires a valid setup!");
	connect(d->engine, &StorageEngine::syncStateChanged,
			this, &SyncController::syncStateChanged,
			Qt::QueuedConnection);
	connect(d->engine, &StorageEngine::syncOperationsChanged,
			this, &SyncController::syncOperationsChanged,
			Qt::QueuedConnection);
	connect(d->engine, &StorageEngine::authenticationErrorChanged,
			this, &SyncController::authenticationErrorChanged,
			Qt::QueuedConnection);
}

SyncController::~SyncController() {}

SyncController::SyncState SyncController::syncState() const
{
	return d->engine->syncState();
}

QString SyncController::authenticationError() const
{
	return d->engine->authenticationError();
}

void SyncController::triggerSync()
{
	QMetaObject::invokeMethod(d->engine, "triggerSync");
}

void SyncController::triggerSyncWithResult(std::function<void (SyncState)> resultFn)
{
	setupTriggerResult(resultFn);
	triggerSync();
}

void SyncController::triggerResync()
{
	QMetaObject::invokeMethod(d->engine, "triggerResync");
}

void SyncController::triggerResyncWithResult(std::function<void (SyncController::SyncState)> resultFn)
{
	setupTriggerResult(resultFn);
	triggerResync();
}

void SyncController::setupTriggerResult(std::function<void (SyncController::SyncState)> resultFn)
{
	auto receiver = new QObject(this);//dummy to disconnect after one call
	connect(this, &SyncController::syncStateChanged, receiver, [resultFn, receiver](SyncState state){
		if(state == Loading || state == Syncing)
			return;
		resultFn(state);
		receiver->deleteLater();
	});
}
