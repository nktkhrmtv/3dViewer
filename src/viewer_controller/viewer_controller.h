#ifndef VIEWER_CONTROLLERH
#define VIEWER_CONTROLLERH

#include "../viewer_model/viewer_model.h"

namespace s21 {

/**
 * @brief Класс контроллера вьювера
 */
class ViewerController {
 public:
  ViewerController(ViewerModel *Model);

  // slots for buttons
  void Model_loadOBJ(const QString &filePath);
  void modelTranslateFigure(const translateAction_t &translateAct,
                            float translateValue);
  void modelRotateAxis(const rotateAction_t &rotateAct, float rotateValue);
  void modelScaleFigure(const ScaleType_t &scaleF, float scaleValue);
  void modelSetProjection(const ProjectionType_t &prog_type);
  void modelSetDefault(int flag);
  void modelSetVerticeType(const VerticeType_t &verticeType);

  void modelSetFacetWidth(const ScaleType_t &scaleF);
  void modelSetVerticeWidth(const ScaleType_t &scaleF);

  void modelSetVerticeColor(const QColor &VerticeColor);
  void modelSetFacetColor(const QColor &FacetColor);
  void modelSetBackGroundColor(const QColor &backGroundColor);

  void modelMouseButtonMove(QPoint delta);
  void modelMouseWheelMove(QPoint delta);
  float modelMakeFloat(QString inputText);

  // getters
  std::vector<QVector3D> modelGetVertices();
  std::vector<unsigned int> modelGetFacets();
  AffineTransform_t modelGetAffineTransform();
  ModelDefinition_t modelGetModelDefinition();

 private:
  ViewerModel *viewer_model;
};

}  // namespace s21

#endif