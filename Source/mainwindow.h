#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>
#include "VolumeManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    static const int ANIMATION_DURATION = 1000;
    const float MAX_OPACITY = 0.75;
    static const int BORDER_RADIUS = 15;
    static const int WINDOW_WIDTH = 400;
    static const int WINDOW_HEIGHT = 400;
	static const int HIDE_TIMEOUT = 3000; 

    Ui::MainWindow *ui;


    QPropertyAnimation *_opacityAnimation;
    bool _windowShown = false;
	VolumeManager* _volumeManager; 
	double _lastVolume; 

	QPixmap _vol0; 
	QPixmap _vol33;
	QPixmap _vol66;
	QPixmap _vol99;

	QTimer* _hideTimer; 


    void Animate(float start, float target);

private slots:
	void Update(); 
	void Hide(); 
};

#endif // MAINWINDOW_H
