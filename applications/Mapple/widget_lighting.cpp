#include "widget_lighting.h"
#include "main_window.h"
#include "paint_canvas.h"
#include <ui_widget_lighting.h>

#include <easy3d/viewer/setting.h>
#include <easy3d/viewer/soft_shadow.h>
#include <easy3d/viewer/ambient_occlusion.h>


using namespace easy3d;

WidgetLighting::WidgetLighting(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetLighting)
{
    ui->setupUi(this);
    viewer_ = dynamic_cast<MainWindow*>(parent)->viewer();

    // two sides lighting
    ui->checkBoxLightTwoSides->setChecked(setting::light_two_sides);
    ui->checkBoxLightDistinctBackColor->setChecked(setting::light_surface_distinct_back_color);
    ui->toolButtonLightDistinctBackColor->setVisible(setting::light_surface_distinct_back_color);
    const vec4& color = setting::light_surface_back_color;
    QPixmap pixmap/*(25, 19)*/;
    pixmap.fill(QColor(color.r * 255, color.g * 255, color.b * 255));
    ui->toolButtonLightDistinctBackColor->setIcon(QIcon(pixmap));

    // SSAO
    ui->comboBoxSSAOAlgorithm->addItem("None");
    ui->comboBoxSSAOAlgorithm->addItem("Classic");
    ui->comboBoxSSAOAlgorithm->addItem("HBO");
    ui->comboBoxSSAOAlgorithm->addItem("HBO+");
    ui->comboBoxSSAOAlgorithm->setCurrentIndex(0);
    ui->horizontalSliderSSAORadius->setValue(setting::effect_ssao_radius * 100);
    ui->horizontalSliderSSAOIntensity->setValue(setting::effect_ssao_intensity * 100);
    ui->horizontalSliderSSAOBias->setValue(setting::effect_ssao_bias * 100);
    ui->horizontalSliderSSAOSharpness->setValue(setting::effect_ssao_sharpness * 100);

    // EDL
    ui->checkBoxEyeDomeLighting->setChecked(setting::effect_edl_enabled);
    ui->checkBoxTransparency->setChecked(setting::effect_transparency_enabled);
    ui->checkBoxShadow->setChecked(setting::effect_shadow_enabled);

    // shadow
    ui->comboBoxShadowSmoothPattern->addItem("Poisson  25 x 25 ");
    ui->comboBoxShadowSmoothPattern->addItem("Poisson  32 x 64 ");
    ui->comboBoxShadowSmoothPattern->addItem("Poisson  64 x 128");
    ui->comboBoxShadowSmoothPattern->addItem("Poisson 100 x 100");
    ui->comboBoxShadowSmoothPattern->addItem("Regular  49 x 225");
    ui->comboBoxShadowSmoothPattern->setCurrentIndex(2);
    connect(ui->comboBoxShadowSmoothPattern, SIGNAL(currentIndexChanged(int)), this, SLOT(setShadowSmoothPattern(int)));

    ui->horizontalSliderShadowLightDistance->setValue(setting::shadow_light_distance);
    ui->horizontalSliderShadowSoftness->setValue(setting::shadow_softness * 100);
    ui->horizontalSliderShadowDarkness->setValue(setting::shadow_darkness * 100);

    //----------------------------------------------

    QPalette::ColorRole role = parent->backgroundRole();
    const QColor& bc = parent->palette().color(role);
    ui->checkerSphere->setBackgroundColor(bc);
    connect(ui->checkerSphere, SIGNAL(lightPositionChanged()), viewer_, SLOT(update()));


    connect(ui->comboBoxSSAOAlgorithm, SIGNAL(currentIndexChanged(int)), this, SLOT(setSSAOAlgorithm(int)));
    connect(ui->horizontalSliderSSAORadius, SIGNAL(valueChanged(int)), this, SLOT(setSSAORadius(int)));
    connect(ui->horizontalSliderSSAOIntensity, SIGNAL(valueChanged(int)), this, SLOT(setSSAOIntensity(int)));
    connect(ui->horizontalSliderSSAOBias, SIGNAL(valueChanged(int)), this, SLOT(setSSAOBias(int)));
    connect(ui->horizontalSliderSSAOSharpness, SIGNAL(valueChanged(int)), this, SLOT(setSSAOSharpness(int)));

    connect(ui->checkBoxEyeDomeLighting, SIGNAL(toggled(bool)), this, SLOT(setEyeDomeLighting(bool)));
    connect(ui->checkBoxTransparency, SIGNAL(toggled(bool)), this, SLOT(setTransparency(bool)));
    connect(ui->checkBoxShadow, SIGNAL(toggled(bool)), this, SLOT(setShadow(bool)));

    connect(ui->checkBoxLightTwoSides, SIGNAL(toggled(bool)), this, SLOT(setTwoSidesLighting(bool)));
    connect(ui->checkBoxLightDistinctBackColor, SIGNAL(toggled(bool)), this, SLOT(setDistinctBackColor(bool)));
    connect(ui->horizontalSliderShadowLightDistance, SIGNAL(valueChanged(int)), this, SLOT(setLightDistance(int)));
    connect(ui->horizontalSliderShadowSoftness, SIGNAL(valueChanged(int)), this, SLOT(setShadowSoftness(int)));
    connect(ui->horizontalSliderShadowDarkness, SIGNAL(valueChanged(int)), this, SLOT(setShadowDarkness(int)));
    connect(ui->toolButtonLightDistinctBackColor, SIGNAL(clicked()), this, SLOT(setBackColor()));
}


WidgetLighting::~WidgetLighting()
{
    delete ui;
}


void WidgetLighting::setSSAOAlgorithm(int algo) {
//    if (algo != AmbientOcclusion_HBAO::SSAO_NONE && ui->checkBoxTransparency->isChecked())
//        ui->checkBoxTransparency->setChecked(false);  // ssao and tranparency cannot co-exist
//    viewer_->ssao()->set_algorithm(AmbientOcclusion_HBAO::Algorithm(algo));
//    viewer_->update();
}


void WidgetLighting::setSSAORadius(int v) {
    viewer_->ssao()->set_radius(v / 100.0f);
    viewer_->update();
}


void WidgetLighting::setSSAOIntensity(int v) {
//    viewer_->ssao()->set_intensity(v / 100.0f);
//    viewer_->update();
}


void WidgetLighting::setSSAOBias(int v) {
    viewer_->ssao()->set_bias(v / 100.0f);
    viewer_->update();
}


void WidgetLighting::setSSAOSharpness(int v) {
//    viewer_->ssao()->set_sharpness(v / 100.0f);
//    viewer_->update();
}


void WidgetLighting::setEyeDomeLighting(bool b) {
    viewer_->enableEyeDomeLighting(b);
    viewer_->update();
}


void WidgetLighting::setTransparency(bool b) {
    if (b && ui->checkBoxShadow->isChecked())
        ui->checkBoxShadow->setChecked(false);  // shadow and tranparency cannot co-exist
//    if (b && (ui->comboBoxSSAOAlgorithm->currentIndex() != 0 || viewer_->ssao()->algorithm() != AmbientOcclusion_HBAO::SSAO_NONE))
//        ui->comboBoxSSAOAlgorithm->setCurrentIndex(0);  // ssao and tranparency cannot co-exist
    viewer_->enableTransparency(b);
    viewer_->update();

}


void WidgetLighting::setShadow(bool b) {
    if (b && ui->checkBoxTransparency->isChecked())
        ui->checkBoxTransparency->setChecked(false);  // shadow and tranparency cannot co-exist
    viewer_->enableShadow(b);
    viewer_->update();
}


void WidgetLighting::setLightDistance(int d) {
    viewer_->shadow()->set_light_distance(d);
    viewer_->update();
}


void WidgetLighting::setShadowSmoothPattern(int v) {
    SoftShadow* shadow = dynamic_cast<SoftShadow*>(viewer_->shadow());
    if (shadow) {
        shadow->set_sample_pattern(SoftShadow::SamplePattern(v));
        viewer_->update();
    }
}


void WidgetLighting::setShadowSoftness(int v) {
    SoftShadow* shadow = dynamic_cast<SoftShadow*>(viewer_->shadow());
    if (shadow) {
        shadow->set_softness(static_cast<float>(v) / 100.0f);
        viewer_->update();
    }
}


void WidgetLighting::setShadowDarkness(int v) {
    viewer_->shadow()->set_darkness(static_cast<float>(v) / 100.0f);
    viewer_->update();
}


void WidgetLighting::setImposterShadows(bool) {

}


void WidgetLighting::setTwoSidesLighting(bool) {

}


void WidgetLighting::setDistinctBackColor(bool) {

}


void WidgetLighting::setBackColor() {

}


