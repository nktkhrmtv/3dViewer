#ifndef VIEWER_VIEWH
#define VIEWER_VIEWH

#include "../viewer_controller/viewer_controller.h"

namespace s21 {

/**
 * @brief Класс, отвечающий за паттерн стратегия для выбора отрисовки в методе
 * OpenGLWidget::paintGL()
 */
class Draw {
 public:
  virtual ~Draw() = default;
  virtual void draw(const std::vector<unsigned int> &facets,
                    const std::vector<QVector3D> &vertices,
                    const ModelDefinition_t &modelDefinition) = 0;
};

/**
 * @brief Класс, отрисовывающий ребра с обычной толщиной
 */
class DrawFacetZero : public Draw {
 public:
  void draw(const std::vector<unsigned int> &facets,
            const std::vector<QVector3D> &vertices,
            const ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс, отрисовывающий ребра с измененной толщиной
 */
class DrawFacetThick : public Draw {
 public:
  void draw(const std::vector<unsigned int> &facets,
            const std::vector<QVector3D> &vertices,
            const ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс, отрисовывающий вершины в форме квадрата
 */
class DrawVerticeSquare : public Draw {
 public:
  void draw(const std::vector<unsigned int> &facets,
            const std::vector<QVector3D> &vertices,
            const ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс, отрисовывающий вершины в форме круга
 */
class DrawVerticeCircle : public Draw {
 public:
  void draw(const std::vector<unsigned int> &facets,
            const std::vector<QVector3D> &vertices,
            const ModelDefinition_t &modelDefinition) override;
};

/**
 * @brief Класс виджета OpenGL
 */
class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
  Q_OBJECT
 public:
  OpenGLWidget(ViewerController *Controller);
  ~OpenGLWidget();

 private:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void drawStrategy(Draw *strategy, const std::vector<unsigned int> &facets,
                    const std::vector<QVector3D> &vertices,
                    const ModelDefinition_t &modelDefinition);

  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  ViewerController *viewer_controller;
  QPoint lastMousePos;

  Draw *draw_;
};

/**
 * @brief Класс, реализующий паттерн команда для афинных преобразований и
 * изменения вершин и ребер
 */
class Command {
 public:
  virtual ~Command() = default;
  virtual void execute() = 0;
};

/**
 * @brief Класс, отвечающий за комманды перемещения по X,Y,Z
 */
class TranslateCommand : public Command {
 public:
  TranslateCommand(ViewerController *controller, float value,
                   translateAction_t translateAction);
  void execute() override;

 private:
  ViewerController *controller_;
  float value;
  translateAction_t trAction;
};

/**
 * @brief Класс, отвечающий за вращение относительно X,Y,Z
 */
class RotateCommand : public Command {
 public:
  RotateCommand(ViewerController *controller, float value,
                rotateAction_t rotateAction);
  void execute() override;

 private:
  ViewerController *controller_;
  float value;
  rotateAction_t rtAction;
};

/**
 * @brief Класс, отвечающий за масштабирование модели
 */
class ScaleCommand : public Command {
 public:
  ScaleCommand(ViewerController *controller, float scaleValue,
               const ScaleType_t &scaleF);
  void execute() override;

 private:
  ViewerController *controller_;
  float sclValue;
  const ScaleType_t sclF;
};

/**
 * @brief Класс, отвечающий за установку цвета ребер
 */
class ChangeFacetColorCommand : public Command {
 public:
  ChangeFacetColorCommand(ViewerController *controller, QColor color);
  void execute() override;

 private:
  ViewerController *controller_;
  QColor color_;
};

/**
 * @brief Класс, отвечающий за установку толщины ребер
 */
class ChangeFacetWidthCommand : public Command {
 public:
  ChangeFacetWidthCommand(ViewerController *controller, ScaleType_t scaleType);
  void execute() override;

 private:
  ViewerController *controller_;
  ScaleType_t scaleType_;
};

/**
 * @brief Класс, отвечающий за установку цвета вершин
 */
class ChangeVerticeColorCommand : public Command {
 public:
  ChangeVerticeColorCommand(ViewerController *controller, QColor color);
  void execute() override;

 private:
  ViewerController *controller_;
  QColor color_;
};

/**
 * @brief Класс, отвечающий за установку толщины вершин
 */
class ChangeVerticeWidthCommand : public Command {
 public:
  ChangeVerticeWidthCommand(ViewerController *controller,
                            ScaleType_t scaleType);
  void execute() override;

 private:
  ViewerController *controller_;
  ScaleType_t scaleType_;
};

/**
 * @brief Класс, реализующий весь интерфейс приложения
 */
class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(ViewerController *Controller);
  ~MainWindow();

 private:
  void part1Buttons();
  void part2Buttons();
  void initializeLayouts(QFont font);
  void initializeButtonLayout(QFont font);
  void initializeConnections();
  void initializeConnections2();

  void saveImage();
  void recordGif();
  void fileOpenButton();
  void defaultModel();
  void setProjection(const ProjectionType_t &projectionType);
  void typeVertice(const VerticeType_t &verticeType);
  void changeBackGroundColor();

  template <typename CommandType, typename DirectionType>
  void executeTransformCommand(QLineEdit *input, DirectionType direction);
  void translate(QLineEdit *input, translateAction_t translateAction);
  void rotate(QLineEdit *input, rotateAction_t rotateAction);
  void scale(QLineEdit *input, ScaleType_t scaleType);

  void executeCommand(std::unique_ptr<Command> command);
  void changeFacetColor();
  void changeFacetWidth(ScaleType_t scaleType);
  void changeVerticeColor();
  void changeVerticeWidth(ScaleType_t scaleType);

  QButtonGroup *groupVertices;
  QHBoxLayout *layout;
  QWidget *centralWidget;
  QButtonGroup *groupProjection;
  QVBoxLayout *layoutFacetSetngs;
  QVBoxLayout *layoutVerticeSetngs;
  QVBoxLayout *layoutProjections;
  QVBoxLayout *translateXLayout;
  QVBoxLayout *translateYLayout;
  QVBoxLayout *rotateXLayout;
  QVBoxLayout *rotateYLayout;
  QVBoxLayout *rotateZLayout;
  QVBoxLayout *scaleLayout;
  QVBoxLayout *layoutSave;

  QPushButton *buttonOpenfile;
  QVBoxLayout *manageLayout;
  QVBoxLayout *buttonLayout;
  QRadioButton *buttonParallel;
  QRadioButton *buttonCentral;
  QPushButton *buttonFacetPlus;
  QPushButton *buttonFacetMinus;
  QPushButton *buttonFacetColor;
  QRadioButton *buttonSquareVertice;
  QRadioButton *buttonCircleVertice;
  QRadioButton *buttonNoneVertice;
  QPushButton *buttonVerticeColor;
  QPushButton *buttonVerticePlus;
  QPushButton *buttonVerticeMinus;
  QPushButton *buttonBackGroundColor;
  QPushButton *buttonSaveImage;
  QPushButton *buttonRecordGif;
  QPushButton *buttonDefault;
  QPushButton *buttonTranslateX_p;
  QPushButton *buttonTranslateX_m;
  QPushButton *buttonTranslateY_p;
  QPushButton *buttonTranslateY_m;
  QPushButton *buttonTranslateZ_p;
  QPushButton *buttonTranslateZ_m;
  QPushButton *buttonRotateX_p;
  QPushButton *buttonRotateX_m;
  QPushButton *buttonRotateY_p;
  QPushButton *buttonRotateY_m;
  QPushButton *buttonRotateZ_p;
  QPushButton *buttonRotateZ_m;
  QPushButton *buttonScale_p;
  QPushButton *buttonScale_m;

  OpenGLWidget *openGL_widget;
  ViewerController *viewer_controller;
  QLabel *label;
  QLineEdit *translateXInput;
  QLineEdit *translateYInput;
  QLineEdit *translateZInput;
  QLineEdit *rotateXInput;
  QLineEdit *rotateYInput;
  QLineEdit *rotateZInput;
  QLineEdit *scaleInput;
  QColor currentFacetColor;
  QColor currentVerticeColor;
  QColor currentBackGroundColor;
};

}  // namespace s21

#endif