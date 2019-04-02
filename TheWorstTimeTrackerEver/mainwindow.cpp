#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

/**
 * @brief Constructor
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  setDefaultTimeIn();
  initializeProgressBar();
  populateComboBox();
  setDefaultLunchBreak();

  connect(ui->calculateTimeButton, &QPushButton::clicked, this, &MainWindow::calculateTime);
  connect(ui->resetButton, &QPushButton::clicked, this, &MainWindow::reset);
} // end constructor

/**
 * @brief Destructor
 */
MainWindow::~MainWindow()
{
  delete ui;
} // end destructor

/**
 * @brief set min and max values of progress bar, then set progress bar to 0
 */
void MainWindow::initializeProgressBar()
{
  this->ui->lengthOfTimeProgressBar->setRange(0, 9); // 9 hour shift
  this->ui->lengthOfTimeProgressBar->reset();
} // end intializeProgressBar

/**
 * @brief populate combo box that goes along with lunch break
 */
void MainWindow::populateComboBox()
{
  this->ui->hoursOrMinutesComboBox->addItem(QString("minute(s)"));
  this->ui->hoursOrMinutesComboBox->addItem(QString("hour(s)"));
} // end populateComboBox

/**
 * @brief set time to 8:30 am
 */
void MainWindow::setDefaultTimeIn()
{
  this->ui->timeInTimeEdit->setTime(QTime(8,30,0,0));
} // end setDefaultTimeIn

/**
 * @brief initialize lunch break to 0.0
 */
void MainWindow::setDefaultLunchBreak()
{
  this->ui->lunchBreakDoubleSpinner->setValue(0.0);
} // end setDefaultLunchBreak

/**
 * @brief reset all of the values back to the default settings
 */
void MainWindow::reset()
{
  this->ui->lengthOfTimeProgressBar->reset();
  setDefaultTimeIn();
  setDefaultLunchBreak();
  resetDurationOfTimeLabel();
  this->ui->yesOrNoLabel->setText(QString("No"));
} // end reset

/**
 * @brief reset text from "You've been here x hours" to "How long have you been here?"
 */
void MainWindow::resetDurationOfTimeLabel()
{
  this->ui->durationOfTimeLabel->setText(QString("How long have you been here?"));
}

/**
 * @brief after calculating the time, set the label to "You've been here for x hours"
 * @param durationOfTime  the length of the time the user has been at a particular location
 */
void MainWindow::setDurationOfTimeLabel(double durationOfTime)
{
  this->ui->durationOfTimeLabel->setText(QString("You've been here for %1 hours").arg(QString::number(durationOfTime, 'f', 1)));
} // end setDurationOfTimeLabel

/**
 * @brief get the current time and subtract it from the lunch break and the time in
 */
void MainWindow::calculateTime()
{
  QTime timeNow = QTime::currentTime();
  QTime timeIn = this->ui->timeInTimeEdit->time();
  if(timeIn > timeNow)
  {
    this->ui->durationOfTimeLabel->setText(QString("Man, time travel would be cool"));
    this->ui->lengthOfTimeProgressBar->reset();
    this->ui->yesOrNoLabel->setText(QString("No"));
    return;
  } // end if timeIn > timeNow

  double hours;
  double minutes;

  /* do some time calculations */
  if(timeIn.minute() > timeNow.minute())
  {
    minutes = (timeNow.minute()/60.0 + 1.0) - timeIn.minute()/60.0;
    hours = timeNow.hour() - timeIn.hour() - 1;
  } else {
    minutes = timeNow.minute()/60.0 - timeIn.minute()/60.0;
    hours = timeNow.hour() - timeIn.hour();
  } // end if timeIn.minute() > timeNow.minute()

  /* factor in the lunch break */
  QString hoursOrMinutes = this->ui->hoursOrMinutesComboBox->currentText();
  double lunchBreak = this->ui->lunchBreakDoubleSpinner->value();
  if(QString::compare(hoursOrMinutes, "minute(s)", Qt::CaseInsensitive) == 0)
  {
    lunchBreak /= 60.0;
    if(lunchBreak > minutes)
    {
      minutes = (minutes+1) - lunchBreak;
      hours -= 1;
      /* tell the user how long (s)he has been there */
      setDurationOfTimeLabel(hours + minutes);
    } else{
      minutes -= lunchBreak;
      /* tell the user how long (s)he has been there */
      setDurationOfTimeLabel(hours + minutes);
    } // end if lunchBreak > minutes
  } else if(QString::compare(hoursOrMinutes, "hour(s)", Qt::CaseInsensitive) == 0){
    double durationStayed = hours + minutes;
    setDurationOfTimeLabel(durationStayed - lunchBreak);
  } // end if QString::compare(hoursOrMinutes, "minute(s)", Qt::CaseInsensitive

  /* now set the progress bar*/
  if(hours >= 9.0)
  {
    this->ui->lengthOfTimeProgressBar->setValue(this->ui->lengthOfTimeProgressBar->maximum());
    this->ui->yesOrNoLabel->setText(QString("Yes"));
  } else {
    this->ui->lengthOfTimeProgressBar->setValue(static_cast<int>(hours));
    this->ui->yesOrNoLabel->setText(QString("No"));
  } // end if hours >= 9.0
} // end calculateTime
