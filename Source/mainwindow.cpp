#include "mainwindow.h"
#include "GeneratedFiles\ui_mainwindow.h"
//#include "ui_mainwindow.h"
#include <QApplication>
#include <qdesktopwidget.h>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QDialog>
#include <QTimer>
#include <qthread.h>
#include <windows.h>




MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	QWidget *hideWidget = new QWidget();
	QDialog *hideDialog = new QDialog(hideWidget);
	hideDialog->show();
	hideDialog->hide();

	_volumeManager.ConnectCallback([this](float f)
	{
		VolumeChanged(f);
	});
	
	_volumeChangedHandle = CreateEvent(
		NULL,               // default security attributes
		TRUE,               // manual-reset event
		FALSE,              // initial state is nonsignaled
		TEXT("VolumeChangedEvent")  // object name
		);

	_hideTimer = new QTimer(this);
	_hideTimer->setSingleShot(true);
	connect(_hideTimer, SIGNAL(timeout()), this, SLOT(Hide()));

	// setting windows options
	Qt::WindowFlags flags = 0;
	flags = Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::ToolTip;
	setWindowFlags(flags);
	setAttribute(Qt::WA_TranslucentBackground);

	QString centralStyle = "background:black;border-radius: %1px;";
	ui->centralWidget->setStyleSheet(centralStyle.arg(BORDER_RADIUS));


	_opacityAnimation = new QPropertyAnimation(this, "windowOpacity", this);
	QSize newSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	setGeometry(QStyle::alignedRect(
		Qt::LeftToRight,
		Qt::AlignCenter,
		newSize,
		qApp->desktop()->availableGeometry()));


	_vol0 = QPixmap("volume_0");
	_vol33 = QPixmap("volume_33");
	_vol66 = QPixmap("volume_66");
	_vol99 = QPixmap("volume_99");


	//QPixmap pm("C:/Users/admin/Desktop/802013058_univ_cnt_4_xl.png");

	const int margin = 25;
	ui->label->setGeometry(margin, 0, WINDOW_WIDTH - margin, WINDOW_WIDTH - margin);
	ui->label->setScaledContents(true);
	ui->label->show();

	ui->progressBar->setGeometry(margin, WINDOW_HEIGHT - 60, WINDOW_WIDTH - (margin * 2), 25);
	ui->progressBar->setAlignment(Qt::AlignCenter);
	ui->progressBar->setTextVisible(false);


	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
	timer->start(50);
}


MainWindow::~MainWindow()
{
	delete ui;
	delete _opacityAnimation;
}


void MainWindow::Animate(float start, float target)
{
	show();
	_opacityAnimation->setDuration(ANIMATION_DURATION);
	_opacityAnimation->setStartValue(start);
	_opacityAnimation->setEndValue(target);
	_opacityAnimation->start();
}


void MainWindow::VolumeChanged(float val)
{
	_lastVolume = val; 
	SetEvent(_volumeChangedHandle);

}
// Slots
void MainWindow::Update()
{
	DWORD res = WaitForSingleObject(_volumeChangedHandle, 5);
	if (res != 0) return;

	_hideTimer->stop();

	ui->progressBar->setValue(_lastVolume);

	if (_lastVolume == 0)
	{
	ui->label->setPixmap(_vol0);
	}
	else if (_lastVolume < 33)
	{
	ui->label->setPixmap(_vol33);
	}
	else if (_lastVolume < 66)
	{
	ui->label->setPixmap(_vol66);
	}
	else
	{
	ui->label->setPixmap(_vol99);
	}


	if (!_windowShown)
	{
	Animate(0, MAX_OPACITY);
	}

	_windowShown = true;
	_hideTimer->start(HIDE_TIMEOUT);

	ResetEvent(_volumeChangedHandle);
}

void MainWindow::Hide()
{
	Animate(MAX_OPACITY, 0);
	_windowShown = false;
}
