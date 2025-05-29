#ifndef VIEWER_MODELH
#define VIEWER_MODELH

#include "strucutures.h"

namespace s21 {

/**
 * @brief Класс, отвечающий за установку цвета
 */
class SetColor {
 public:
  virtual ~SetColor() = default;
  virtual void setColor(const QColor &color,
                        ModelDefinition_t &modelDefinition) = 0;
};

/**
 * @brief Класс для установки цвета ребер модели
 */
class SetFacetColor : public SetColor {
 public:
  void setColor(const QColor &facetColor,
                ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс для установки цвета вершин модели
 */
class SetVerticeColor : public SetColor {
 public:
  void setColor(const QColor &verticeColor,
                ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс для установки цвета фона модели
 */
class SetBackGroundColor : public SetColor {
 public:
  void setColor(const QColor &backGroundColor,
                ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс модели вьювера
 *
 * Этот класс управляет представлением модели, обеспечивая методы для
 * трансформации, установки цвета, загрузки моделей и выполнения других
 * операций, связанных с визуализацией
 */
class ViewerModel {
 public:
  ViewerModel();
  ~ViewerModel();

  void translateFigure(const translateAction_t &translateAct,
                       float translateValue);
  void rotateAxis(const rotateAction_t &rotateAct, float rotateValue);
  void setProjection(const ProjectionType_t &prog_type);
  void setDefault(int flag);
  void setVerticeType(const VerticeType_t &verticeType);
  void scaleFigure(const ScaleType_t &scaleF, float scaleValue);
  void setFacetWidth(const ScaleType_t &scaleF);
  void setVerticeWidth(const ScaleType_t &scaleF);

  void SetColorStrategy(SetColor *strategy, const QColor &color);
  void setFacetColor(const QColor &FacetColor);
  void setVerticeColor(const QColor &VerticeColor);
  void setBackGroundColor(const QColor &backGroundColor);

  std::vector<QVector3D> getVertices();
  std::vector<unsigned int> getFacets();
  AffineTransform_t getAffineTransform();
  ModelDefinition_t getModelDefinition();

  void MouseButtonMove(QPoint delta);
  void MouseWheelMove(QPoint delta);
  void loadOBJ(const QString &filePath);
  float makeFloat(const QString &inputText);

  // in public section for tests
  void normalizeVertices();
  void saveModelDefinition();
  void readModelDefinition();

 private:
  std::vector<QVector3D> vertices;
  std::vector<unsigned int> facets;
  AffineTransform_t affine_transform;
  ModelDefinition_t modelDefinition;
  SetColor *setColor_;
};

}  // namespace s21

#endif