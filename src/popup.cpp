#include "popup.h"
#include <QTimer>
#include <QPropertyAnimation>

const int WIDGET_MIN_WIDTH = 450;
const int WIDGET_MIN_HEIGHT = 75;
const int IMAGE_WIDTH = 150;
const QString PADDING = "5px 10px";

const int ANIMATION_DURATION = 200;
const int ANIMATION_OFFSET = 150;
const int HIDE_DELAY = 4000;

Popup::Popup(Settings * settings, QWidget *parent) : QWidget(parent)
{
    setMinimumWidth(WIDGET_MIN_WIDTH);
    setMinimumHeight(WIDGET_MIN_HEIGHT);

    std::string tooltipBgColorStr = settings->get("editor_tooltip_bg_color");
    std::string tooltipColorStr = settings->get("editor_tooltip_color");
    std::string widgetBorderColorStr = settings->get("editor_widget_border_color");

    hLayout = new QHBoxLayout(this);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);

    imgLabel = new QLabel();
    QPixmap pm(":/image/vanko1");
    imgLabel->setPixmap(pm);
    imgLabel->setScaledContents(false);
    imgLabel->setFixedWidth(IMAGE_WIDTH);
    hLayout->addWidget(imgLabel);

    textLabel = new QLabel();
    textLabel->setTextFormat(Qt::RichText);
    textLabel->setWordWrap(true);
    textLabel->setAlignment(Qt::AlignTop);
    textLabel->setMinimumWidth(WIDGET_MIN_WIDTH - IMAGE_WIDTH);
    textLabel->setStyleSheet("background:"+QString::fromStdString(tooltipBgColorStr)+";color:"+QString::fromStdString(tooltipColorStr)+";padding:"+PADDING+";font-weight:bold;");
    hLayout->addWidget(textLabel);

    hLayout->addStretch();

    setStyleSheet("background:"+QString::fromStdString(tooltipBgColorStr)+";border:1px solid "+QString::fromStdString(widgetBorderColorStr)+";");
    hide();
}

Popup::~Popup()
{

}

QSize Popup::sizeHint() const {
    return QSize(WIDGET_MIN_WIDTH, WIDGET_MIN_HEIGHT);
}

void Popup::mousePressEvent(QMouseEvent * /*e*/)
{
    hide();
}

void Popup::hide()
{
    setVisible(false);
    animationInProgress = false;
}

void Popup::animateIn()
{
    QRect rect = geometry();

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(QRect(rect.x(), rect.y()-ANIMATION_OFFSET, rect.width(), rect.height()));
    animation->setEndValue(QRect(rect.x(), rect.y(), rect.width(), rect.height()));

    QEasingCurve easing(QEasingCurve::OutCubic);
    animation->setEasingCurve(easing);

    animation->start();
}

void Popup::animateOut()
{
    QRect rect = geometry();

    QPropertyAnimation *animation = new QPropertyAnimation(this, "geometry");
    animation->setDuration(ANIMATION_DURATION);
    animation->setStartValue(QRect(rect.x(), rect.y(), rect.width(), rect.height()));
    animation->setEndValue(QRect(rect.x(), rect.y()-ANIMATION_OFFSET, rect.width(), rect.height()));

    QEasingCurve easing(QEasingCurve::InCubic);
    animation->setEasingCurve(easing);

    animation->start();
}

void Popup::display(int x, int y, QString text)
{
    QRect rect = geometry();
    rect.setTop(y);
    rect.setLeft(x);
    setGeometry(rect);
    textLabel->setText(text);

    setVisible(true);
    raise();

    if (!animationInProgress) {
        animationInProgress = true;
        animateIn();
        QTimer::singleShot(HIDE_DELAY-2*ANIMATION_DURATION, this, SLOT(animateOut()));
        QTimer::singleShot(HIDE_DELAY, this, SLOT(hide()));
    }
}

void Popup::displayText(int x, int y, QString text)
{
    QPixmap pm(":/image/vanko2");
    imgLabel->setPixmap(pm);

    display(x, y, text);
}

void Popup::displayError(int x, int y, QString text)
{
    QPixmap pm(":/image/vanko1");
    imgLabel->setPixmap(pm);

    display(x, y, text);
}
