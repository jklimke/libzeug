#pragma once

#include <memory>

#include <QDialog>

#include <widgetzeug/widgetzeug_api.h>
#include <widgetzeug/ColorGradient.h>

class Ui_ColorGradientWidget;

namespace widgetzeug
{

class ColorGradientModel;

class WIDGETZEUG_API ColorGradientWidget : public QDialog
{
    Q_OBJECT

public:
    ColorGradientWidget(
        const ColorGradient & gradient,
        QWidget * parent = nullptr);
    
    ~ColorGradientWidget();
    
    ColorGradient gradient() const;
    
    void setHistogram(const QList<uint> & histogram);
    
protected:
    void loadFromJson();
    void saveToJson();
    
private:
    void updateStepsState();
    void setModel(std::unique_ptr<ColorGradientModel> && model);
    
private:
    std::unique_ptr<ColorGradientModel> m_model;
    Ui_ColorGradientWidget * m_ui;
};

} // namespace widgetzeug
