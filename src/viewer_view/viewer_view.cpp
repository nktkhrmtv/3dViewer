#include "viewer_view.h"

namespace s21 {

/**
 * @brief Конструктор OpenGL-виджета.
 *
 * Инициализирует OpenGL-виджет, устанавливая указатель на контроллер
 * представления.
 *
 * @param Controller Указатель на контроллер представления.
 */
OpenGLWidget::OpenGLWidget(ViewerController *Controller)
    : viewer_controller(Controller), draw_(nullptr){};

/**
 * @brief Деструктор OpenGL-виджета.
 *
 * Освобождает память, выделенную под объект `draw_`, если он был создан.
 */
OpenGLWidget::~OpenGLWidget() {
  if (draw_) {
    delete draw_;
  }
}

/**
 * @brief Инициализация OpenGL, настройка функций и включение глубинного теста.
 *
 * Настроены параметры, необходимые для работы OpenGL, такие как глубинный тест
 * и цвет фона.
 */
void OpenGLWidget::initializeGL() {
  initializeOpenGLFunctions();
  glEnable(GL_DEPTH_TEST);
  glClearColor(0.1f, 0.1f, 0.5f, 1.0f);
}

/**
 * @brief Настройка области просмотра для OpenGL.
 *
 * Изменяет область отображения в соответствии с размером окна. Вызывается при
 * изменении размера окна.
 *
 * @param w Ширина нового окна.
 * @param h Высота нового окна.
 */
void OpenGLWidget::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

/**
 * @brief Установка стратегии отрисовки объектов.
 *
 * Смена текущей стратегии отрисовки, основанной на переданных параметрах.
 * Удаляется предыдущая стратегия, если она была установлена.
 *
 * @param strategy Указатель на стратегию отрисовки.
 * @param facets Список индексов граней, используемых для отрисовки.
 * @param vertices Список вершин, используемых для отрисовки.
 * @param modelDefinition Определение модели, содержащее параметры отрисовки.
 */
void OpenGLWidget::drawStrategy(Draw *strategy,
                                const std::vector<unsigned int> &facets,
                                const std::vector<QVector3D> &vertices,
                                const ModelDefinition_t &modelDefinition) {
  if (draw_) {
    delete draw_;
  }
  draw_ = strategy;
  draw_->draw(facets, vertices, modelDefinition);
}

/**
 * @brief Отрисовка граней с обычной толщиной линии.
 *
 * Стандартная отрисовка граней модели, где каждая грань представляется линией,
 * соединяющей вершины в контуре.
 *
 * @param facets Список индексов граней, используемых для отрисовки.
 * @param vertices Список вершин, используемых для отрисовки.
 * @param modelDefinition Определение модели, содержащее параметры отрисовки.
 */
void DrawFacetZero::draw(const std::vector<unsigned int> &facets,
                         const std::vector<QVector3D> &vertices,
                         const ModelDefinition_t &modelDefinition) {
  (void)modelDefinition;
  glBegin(GL_LINE_LOOP);  // указывает, что надо отрисовать замкнутый контур
                          // соединия по 2 вершины
  for (unsigned int facet : facets) {
    if (facet < vertices.size()) {
      const QVector3D &v = vertices[facet];  // координаты вершины грани
      glVertex3f(v.x(), v.y(), v.z());
    }
  }
  glEnd();
}

/**
 * @brief Отрисовка граней с увеличенной толщиной линии.
 *
 * В отличие от стандартной отрисовки, здесь каждая грань отрисовывается как
 * прямоугольник с вычисленной перпендикулярной толщиной линии.
 *
 * @param facets Список индексов граней, используемых для отрисовки.
 * @param vertices Список вершин, используемых для отрисовки.
 * @param modelDefinition Определение модели, содержащее параметры отрисовки.
 */
void DrawFacetThick::draw(const std::vector<unsigned int> &facets,
                          const std::vector<QVector3D> &vertices,
                          const ModelDefinition_t &modelDefinition) {
  for (size_t i = 0; i < facets.size(); ++i) {
    unsigned int facet1 = facets[i];
    unsigned int facet2 = facets[(i + 1) % facets.size()];  // Замыкаем контур
    if (facet1 < vertices.size() && facet2 < vertices.size()) {
      const QVector3D &v1 = vertices[facet1];
      const QVector3D &v2 = vertices[facet2];
      QVector3D direction = v2 - v1;
      QVector3D arbitraryVector(1, 0, 0);  // Произвольный вектор
      // Если направление линии коллинеарно с произвольным вектором, выбираем
      // другой вектор
      if (QVector3D::dotProduct(direction.normalized(),
                                arbitraryVector.normalized()) > 0.99f) {
        arbitraryVector = QVector3D(0, 1, 0);
      }
      // Вычисляем перпендикулярный вектор
      QVector3D perpendicular =
          QVector3D::crossProduct(direction, arbitraryVector).normalized() *
          modelDefinition.facetWidth / 2.0f;
      // Вершины прямоугольника
      QVector3D p1 = v1 - perpendicular;
      QVector3D p2 = v1 + perpendicular;
      QVector3D p3 = v2 + perpendicular;
      QVector3D p4 = v2 - perpendicular;
      // Отрисовка прямоугольника
      glBegin(GL_QUADS);
      glVertex3f(p1.x(), p1.y(), p1.z());
      glVertex3f(p2.x(), p2.y(), p2.z());
      glVertex3f(p3.x(), p3.y(), p3.z());
      glVertex3f(p4.x(), p4.y(), p4.z());
      glEnd();
    }
  }
}

/**
 * @brief Отрисовка вершин модели в виде квадратных точек.
 *
 * Каждая вершина модели представляется как квадрат, размер которого
 * определяется параметром отрисовки.
 *
 * @param facets Список индексов граней, используемых для отрисовки.
 * @param vertices Список вершин, используемых для отрисовки.
 * @param modelDefinition Определение модели, содержащее параметры отрисовки.
 */
void DrawVerticeSquare::draw(const std::vector<unsigned int> &facets,
                             const std::vector<QVector3D> &vertices,
                             const ModelDefinition_t &modelDefinition) {
  (void)facets;
  glPointSize(modelDefinition.verticeWidth);
  glBegin(GL_POINTS);
  for (const QVector3D &v : vertices) {
    glVertex3f(v.x(), v.y(), v.z());
  }
  glEnd();
}

/**
 * @brief Отрисовка вершин модели в виде круговых точек.
 *
 * Каждая вершина модели представляется как круг, размер которого определяется
 * параметром отрисовки.
 *
 * @param facets Список индексов граней, используемых для отрисовки.
 * @param vertices Список вершин, используемых для отрисовки.
 * @param modelDefinition Определение модели, содержащее параметры отрисовки.
 */
void DrawVerticeCircle::draw(const std::vector<unsigned int> &facets,
                             const std::vector<QVector3D> &vertices,
                             const ModelDefinition_t &modelDefinition) {
  (void)facets;
  float radius = modelDefinition.verticeWidth / 1000.0f;  // Радиус круга
  for (const QVector3D &v : vertices) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(v.x(), v.y(), v.z());
    for (int i = 0; i <= 36; ++i) {
      float angle = 2.0f * M_PI * i / 36;
      float x = v.x() + radius * cos(angle);
      float y = v.y() + radius * sin(angle);
      glVertex3f(x, y, v.z());
    }
    glEnd();
  }
}

/**
 * @brief Отрисовка модели в соответствии с заданными аффинными
 * преобразованиями.
 *
 * Выполняется очищение буферов, установка матрицы проекции и матрицы модели, а
 * также трансформации модели (масштабирование, повороты, сдвиги). Затем
 * отрисовываются грани и вершины модели.
 */
void OpenGLWidget::paintGL() {
  AffineTransform_t affineTransform =
      viewer_controller->modelGetAffineTransform();
  ModelDefinition_t modelDefinition =
      viewer_controller->modelGetModelDefinition();
  glClearColor(modelDefinition.backgroundColor.redF(),
               modelDefinition.backgroundColor.greenF(),
               modelDefinition.backgroundColor.blueF(), 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // очистка буферов
  // настройка проекции
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();  // сбрасывает текущую матрицу проекции к единичной матрице
  // текущая проекция
  if (affineTransform.projectionType == Parallel) {
    glOrtho(-2, 2, -2, 2, -2, 2);
  } else if (affineTransform.projectionType == Perspective) {
    float aspectRatio =
        static_cast<float>(width()) / static_cast<float>(height());
    gluPerspective(affineTransform.fov, aspectRatio, 0.01f, 100.0f);
  }
  // режим представления модели
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();  // сбрасывает текущую матрицу модели к единичной матрице
  // аффинные преобразования
  glScalef(affineTransform.scaleFactor, affineTransform.scaleFactor,
           affineTransform.scaleFactor);
  glRotatef(affineTransform.rotateAngleX, 1.0f, 0.0f, 0.0f);
  glRotatef(affineTransform.rotateAngleY, 0.0f, 1.0f, 0.0f);
  glRotatef(affineTransform.rotateAngleZ, 0.0f, 0.0f, 1.0f);
  glTranslatef(affineTransform.translateX, affineTransform.translateY,
               affineTransform.translateZ);

  std::vector<unsigned int> facets = viewer_controller->modelGetFacets();
  std::vector<QVector3D> vertices = viewer_controller->modelGetVertices();
  glColor3f(modelDefinition.facetColor.redF(),
            modelDefinition.facetColor.greenF(),
            modelDefinition.facetColor.blueF());
  // Отрисовка ребер с неизмененным кнопками размером
  if (modelDefinition.facetWidth == 0)
    drawStrategy(new DrawFacetZero, facets, vertices, modelDefinition);
  else
    drawStrategy(new DrawFacetThick, facets, vertices, modelDefinition);
  // отрисовка вершин в виде точек
  glColor3f(modelDefinition.verticeColor.redF(),
            modelDefinition.verticeColor.greenF(),
            modelDefinition.verticeColor.blueF());
  if (modelDefinition.verticeType == Square)
    drawStrategy(new DrawVerticeSquare, facets, vertices, modelDefinition);
  else if (modelDefinition.verticeType == Circle)
    drawStrategy(new DrawVerticeCircle, facets, vertices, modelDefinition);
}

/**
 * @brief Обработка нажатия кнопки мыши для установки начальной позиции мыши.
 *
 * @param event Событие нажатия кнопки мыши, содержащее информацию о кнопке и её
 * позиции.
 */
void OpenGLWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    lastMousePos = event->pos();
  } else if (event->button() == Qt::MiddleButton) {
    lastMousePos = event->pos();
  }
}

/**
 * @brief Обработка движения мыши при зажатой кнопке, выполняется перемещение
 * или вращение модели.
 *
 * @param event Событие перемещения мыши, содержащее информацию о текущей
 * позиции мыши.
 */
void OpenGLWidget::mouseMoveEvent(QMouseEvent *event) {
  QPoint delta = event->pos() - lastMousePos;
  if (event->buttons() & Qt::LeftButton) {
    viewer_controller->modelMouseButtonMove(delta);
  } else if (event->buttons() & Qt::MiddleButton) {
    viewer_controller->modelMouseWheelMove(delta);
  }
  lastMousePos = event->pos();
  update();
}

/**
 * @brief Обработка колесика мыши для изменения масштаба модели.
 *
 * @param event Событие колесика мыши, содержащее информацию о направлении
 * прокрутки.
 */
void OpenGLWidget::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 8;
  if (!numDegrees.isNull()) {
    if (numDegrees.y() > 0)
      viewer_controller->modelScaleFigure(scalePlus, 0.0f);
    else
      viewer_controller->modelScaleFigure(scaleMinus, 0.0f);
    update();
  }
}

/**
 * @brief Конструктор главного окна, инициализирует все компоненты интерфейса.
 *
 * @param Controller Объект для управления взаимодействием с моделью.
 */
MainWindow::MainWindow(ViewerController *Controller)
    : viewer_controller(Controller) {
  openGL_widget = new OpenGLWidget(viewer_controller);
  QFont font("Arial", 12, QFont::Bold);
  setWindowTitle("3DViewer");
  resize(800, 600);
  // Создаем центральный виджет
  centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  // Создаем label для информации о файле
  label = new QLabel();
  label->setWordWrap(true);
  label->setFixedSize(185, 250);
  label->setStyleSheet("QLabel { border: 2px solid black; padding: 5px; }");
  label->setText(QString("file:\n \nvertices:\n \nfacets: "));
  buttonOpenfile = new QPushButton("Choose File");

  // Создаем горизонтальный контейнер для компоновки
  layout = new QHBoxLayout(centralWidget);
  // Создаем вертикальный контейнер для информации о файле
  manageLayout = new QVBoxLayout();
  layout->addLayout(manageLayout);
  // Добавляем виджет openGL
  layout->addWidget(openGL_widget, 1);
  // Создаем вертикальный контейнер для кнопок
  buttonLayout = new QVBoxLayout();
  // Добавляем вертикальный контейнер с кнопками в вертикальный контейнер
  layout->addLayout(buttonLayout);

  part1Buttons();
  part2Buttons();
  initializeLayouts(font);
  initializeButtonLayout(font);
  initializeConnections();
  initializeConnections2();
};

/**
 * @brief Инициализация кнопок для первой части интерфейса (трансляция,
 * вращение, масштабирование).
 */
void MainWindow::part1Buttons() {
  buttonTranslateX_p = new QPushButton("translate X+");
  buttonTranslateY_p = new QPushButton("translate Y+");
  buttonTranslateZ_p = new QPushButton("translate Z+");
  buttonRotateX_p = new QPushButton("Rotate X+");
  buttonRotateY_p = new QPushButton("Rotate Y+");
  buttonRotateZ_p = new QPushButton("Rotate Z+");
  buttonTranslateX_m = new QPushButton("translate X-");
  buttonTranslateY_m = new QPushButton("translate Y-");
  buttonTranslateZ_m = new QPushButton("translate Z-");
  buttonRotateX_m = new QPushButton("Rotate X-");
  buttonRotateY_m = new QPushButton("Rotate Y-");
  buttonRotateZ_m = new QPushButton("Rotate Z-");
  buttonScale_p = new QPushButton("Scale+");
  buttonScale_m = new QPushButton("Scale-");
  buttonDefault = new QPushButton("Default Projection");
  translateXInput = new QLineEdit();
  translateYInput = new QLineEdit();
  translateZInput = new QLineEdit();
  rotateXInput = new QLineEdit();
  rotateYInput = new QLineEdit();
  rotateZInput = new QLineEdit();
  scaleInput = new QLineEdit();
  translateXInput->setPlaceholderText("Enter X value");
  translateYInput->setPlaceholderText("Enter Y value");
  translateZInput->setPlaceholderText("Enter Z value");
  rotateXInput->setPlaceholderText("Enter X angle");
  rotateYInput->setPlaceholderText("Enter Y angle");
  rotateZInput->setPlaceholderText("Enter Z angle");
  scaleInput->setPlaceholderText("Enter Scale");
}

/**
 * @brief Инициализация кнопок для второй части интерфейса (параметры проекций,
 * фасеты и вершины).
 */
void MainWindow::part2Buttons() {
  // Кнопки для part2
  buttonParallel = new QRadioButton("Parallel Projection");
  buttonCentral = new QRadioButton("Central Projection");
  groupProjection = new QButtonGroup();
  groupProjection->addButton(buttonParallel);
  groupProjection->addButton(buttonCentral);
  buttonFacetPlus = new QPushButton("Facets Width+");
  buttonFacetMinus = new QPushButton("Facets Width-");
  buttonFacetColor = new QPushButton("Facets Color");
  buttonSquareVertice = new QRadioButton("Square Vertices");
  buttonCircleVertice = new QRadioButton("Circle Vertices");
  buttonNoneVertice = new QRadioButton("Hide Vertices");
  groupVertices = new QButtonGroup();
  groupVertices->addButton(buttonSquareVertice);
  groupVertices->addButton(buttonCircleVertice);
  groupVertices->addButton(buttonNoneVertice);
  buttonVerticeColor = new QPushButton("Vertices Color");
  buttonVerticePlus = new QPushButton("Vertices Width+");
  buttonVerticeMinus = new QPushButton("Vertices Width-");
  buttonBackGroundColor = new QPushButton("BackGround Color");
  // Кнопки для part3
  buttonSaveImage = new QPushButton("Save Image");
  buttonRecordGif = new QPushButton("Record GIF");
}

/**
 * @brief Инициализация компоновки для настроек фасетов, вершин и проекций.
 *
 * @param font Шрифт для кнопок.
 */
void MainWindow::initializeLayouts(QFont font) {
  // Вертикальный контейнер для всех настроек ребер
  layoutFacetSetngs = new QVBoxLayout();
  layoutFacetSetngs->addWidget(buttonFacetColor);
  buttonFacetColor->setFont(font);
  layoutFacetSetngs->addWidget(buttonFacetPlus);
  buttonFacetPlus->setFont(font);
  layoutFacetSetngs->addWidget(buttonFacetMinus);
  buttonFacetMinus->setFont(font);

  // Вертикальный контейнер для всех настроек вершин
  layoutVerticeSetngs = new QVBoxLayout();
  layoutVerticeSetngs->addWidget(buttonVerticeColor);
  buttonVerticeColor->setFont(font);
  layoutVerticeSetngs->addWidget(buttonVerticePlus);
  buttonVerticePlus->setFont(font);
  layoutVerticeSetngs->addWidget(buttonVerticeMinus);
  buttonVerticeMinus->setFont(font);
  layoutVerticeSetngs->addWidget(buttonCircleVertice);
  buttonCircleVertice->setFont(font);
  layoutVerticeSetngs->addWidget(buttonSquareVertice);
  buttonSquareVertice->setFont(font);
  layoutVerticeSetngs->addWidget(buttonNoneVertice);
  buttonNoneVertice->setFont(font);

  // Вертикальный контейнер для проекций
  layoutProjections = new QVBoxLayout();
  layoutProjections->addWidget(buttonParallel);
  buttonParallel->setFont(font);
  layoutProjections->addWidget(buttonCentral);
  buttonCentral->setFont(font);

  // Группируем левые кнопки в manageLayout
  manageLayout->addWidget(buttonOpenfile);
  buttonOpenfile->setFont(font);
  manageLayout->addWidget(label);
  label->setFont(font);
  manageLayout->addWidget(buttonBackGroundColor);
  buttonBackGroundColor->setFont(font);
  manageLayout->addLayout(layoutFacetSetngs);
  manageLayout->addLayout(layoutVerticeSetngs);
  manageLayout->addLayout(layoutProjections);
}

/**
 * @brief Заполнение buttonLayout кнопками для трансляции, вращения и
 * масштабирования.
 *
 * @param font Шрифт для кнопок.
 */
void MainWindow::initializeButtonLayout(QFont font) {
  buttonLayout->addWidget(buttonDefault);
  buttonDefault->setFont(font);
  translateXLayout = new QVBoxLayout();
  translateXLayout->addWidget(buttonTranslateX_p);
  translateXLayout->addWidget(buttonTranslateX_m);
  translateXLayout->addWidget(translateXInput);
  buttonLayout->addLayout(translateXLayout);
  translateYLayout = new QVBoxLayout();
  translateYLayout->addWidget(buttonTranslateY_p);
  translateYLayout->addWidget(buttonTranslateY_m);
  translateYLayout->addWidget(translateYInput);
  buttonLayout->addLayout(translateYLayout);
  QVBoxLayout *translateZLayout = new QVBoxLayout();
  translateZLayout->addWidget(buttonTranslateZ_p);
  translateZLayout->addWidget(buttonTranslateZ_m);
  translateZLayout->addWidget(translateZInput);
  buttonLayout->addLayout(translateZLayout);
  rotateXLayout = new QVBoxLayout();
  rotateXLayout->addWidget(buttonRotateX_p);
  rotateXLayout->addWidget(buttonRotateX_m);
  rotateXLayout->addWidget(rotateXInput);
  buttonLayout->addLayout(rotateXLayout);
  rotateYLayout = new QVBoxLayout();
  rotateYLayout->addWidget(buttonRotateY_p);
  rotateYLayout->addWidget(buttonRotateY_m);
  rotateYLayout->addWidget(rotateYInput);
  buttonLayout->addLayout(rotateYLayout);
  rotateZLayout = new QVBoxLayout();
  rotateZLayout->addWidget(buttonRotateZ_p);
  rotateZLayout->addWidget(buttonRotateZ_m);
  rotateZLayout->addWidget(rotateZInput);
  buttonLayout->addLayout(rotateZLayout);
  scaleLayout = new QVBoxLayout();
  scaleLayout->addWidget(buttonScale_p);
  scaleLayout->addWidget(buttonScale_m);
  scaleLayout->addWidget(scaleInput);
  buttonLayout->addLayout(scaleLayout);
  layoutSave = new QVBoxLayout();
  layoutSave->addWidget(buttonSaveImage);
  buttonSaveImage->setFont(font);
  layoutSave->addWidget(buttonRecordGif);
  buttonRecordGif->setFont(font);
  buttonLayout->addLayout(layoutSave);
}

/**
 * @brief Инициализирует соединения для кнопок трансформации модели.
 *
 * @param Нет параметров.
 * @details Устанавливает сигнально-слотовые соединения для обработки
 * нажатий на кнопки трансляции, вращения, масштабирования и сброса модели.
 */
void MainWindow::initializeConnections() {
  // подключение методов 1 пункт
  connect(buttonOpenfile, &QPushButton::clicked, this,
          &MainWindow::fileOpenButton);
  connect(buttonTranslateX_p, &QPushButton::clicked, this,
          [this]() { translate(translateXInput, translateXPlus); });
  connect(buttonTranslateX_m, &QPushButton::clicked, this,
          [this]() { translate(translateXInput, translateXMinus); });
  connect(buttonTranslateY_p, &QPushButton::clicked, this,
          [this]() { translate(translateYInput, translateYPlus); });
  connect(buttonTranslateY_m, &QPushButton::clicked, this,
          [this]() { translate(translateYInput, translateYMinus); });
  connect(buttonTranslateZ_p, &QPushButton::clicked, this,
          [this]() { translate(translateZInput, translateZPlus); });
  connect(buttonTranslateZ_m, &QPushButton::clicked, this,
          [this]() { translate(translateZInput, translateZMinus); });
  connect(buttonRotateX_p, &QPushButton::clicked, this,
          [this]() { rotate(rotateXInput, rotateXPlus); });
  connect(buttonRotateX_m, &QPushButton::clicked, this,
          [this]() { rotate(rotateXInput, rotateXMinus); });
  connect(buttonRotateY_p, &QPushButton::clicked, this,
          [this]() { rotate(rotateYInput, rotateYPlus); });
  connect(buttonRotateY_m, &QPushButton::clicked, this,
          [this]() { rotate(rotateYInput, rotateYMinus); });
  connect(buttonRotateZ_p, &QPushButton::clicked, this,
          [this]() { rotate(rotateZInput, rotateZPlus); });
  connect(buttonRotateZ_m, &QPushButton::clicked, this,
          [this]() { rotate(rotateZInput, rotateZMinus); });
  connect(buttonScale_p, &QPushButton::clicked, this,
          [this]() { scale(scaleInput, scalePlus); });
  connect(buttonScale_m, &QPushButton::clicked, this,
          [this]() { scale(scaleInput, scaleMinus); });
  connect(buttonDefault, &QPushButton::clicked, this,
          &MainWindow::defaultModel);
}

/**
 * @brief Инициализирует соединения для кнопок управления отображением.
 *
 * @param Нет параметров.
 * @details Устанавливает соединения для изменения проекции, цвета граней,
 * толщины граней, цвета вершин, толщины вершин и типа вершин.
 */
void MainWindow::initializeConnections2() {
  // подключение методов 2 пункт
  connect(buttonParallel, &QRadioButton::clicked, this,
          [this]() { setProjection(Parallel); });
  connect(buttonCentral, &QRadioButton::clicked, this,
          [this]() { setProjection(Perspective); });
  connect(buttonFacetColor, &QRadioButton::clicked, this,
          &MainWindow::changeFacetColor);
  connect(buttonFacetPlus, &QRadioButton::clicked, this,
          [this]() { changeFacetWidth(scalePlus); });
  connect(buttonFacetMinus, &QRadioButton::clicked, this,
          [this]() { changeFacetWidth(scaleMinus); });
  connect(buttonVerticeColor, &QRadioButton::clicked, this,
          &MainWindow::changeVerticeColor);
  connect(buttonVerticePlus, &QRadioButton::clicked, this,
          [this]() { changeVerticeWidth(scalePlus); });
  connect(buttonVerticeMinus, &QRadioButton::clicked, this,
          [this]() { changeVerticeWidth(scaleMinus); });
  connect(buttonCircleVertice, &QRadioButton::clicked, this,
          [this]() { typeVertice(Circle); });
  connect(buttonSquareVertice, &QRadioButton::clicked, this,
          [this]() { typeVertice(Square); });
  connect(buttonNoneVertice, &QRadioButton::clicked, this,
          [this]() { typeVertice(None); });
  connect(buttonBackGroundColor, &QRadioButton::clicked, this,
          &MainWindow::changeBackGroundColor);
  // Подключение методов 3 пункт
  connect(buttonSaveImage, &QPushButton::clicked, this, &MainWindow::saveImage);
  connect(buttonRecordGif, &QPushButton::clicked, this, &MainWindow::recordGif);
}

/**
 * @brief Деструктор MainWindow, освобождает выделенные ресурсы.
 *
 * @param Нет параметров.
 * @details Удаляет все созданные в конструкторе компоненты и освобождает
 * память.
 */
MainWindow::~MainWindow() {
  delete openGL_widget;
  delete groupVertices;
  delete groupProjection;
  delete layoutFacetSetngs;
  delete layoutVerticeSetngs;
  delete layoutProjections;
  delete translateXLayout;
  delete translateYLayout;
  delete rotateXLayout;
  delete rotateYLayout;
  delete rotateZLayout;
  delete scaleLayout;
  delete layoutSave;
}

/**
 * @brief Сохраняет изображение с OpenGL-виджета в BMP и JPEG.
 *
 * @param Нет параметров.
 * @details Позволяет пользователю выбрать путь сохранения файлов.
 * В случае успешного сохранения изображение записывается в указанном формате.
 */
void MainWindow::saveImage() {
  QImage image = openGL_widget->grabFramebuffer();
  QString bmpFilePath = QFileDialog::getSaveFileName(this, "Save BMP File", "",
                                                     "BMP Files (*.bmp)");
  if (!bmpFilePath.isEmpty()) {
    image.save(bmpFilePath, "BMP");
  }
  QString jpegFilePath = QFileDialog::getSaveFileName(this, "Save JPEG File",
                                                      "", "JPEG Files (*.jpg)");
  if (!jpegFilePath.isEmpty()) {
    image.save(jpegFilePath, "JPEG");
  }
}

/**
 * @brief Записывает анимацию в формате GIF.
 *
 * @param Нет параметров.
 * @details Делает 50 кадров с задержкой 100 мс и создает GIF с помощью FFmpeg.
 * Если FFmpeg не удается запустить, выводит сообщение об ошибке.
 */
void MainWindow::recordGif() {
  QString gifFilePath = QFileDialog::getSaveFileName(this, "Save GIF File", "",
                                                     "GIF Files (*.gif)");
  QFileInfo gifFileInfo(gifFilePath);
  QString gifDir = gifFileInfo.absoluteDir().absolutePath();
  QString gifFileName = gifFileInfo.baseName();
  QString framesDir = gifDir + "/" + gifFileName + "_frames";
  QDir().mkpath(framesDir);

  for (int i = 0; i < 50; ++i) {
    QImage frame = openGL_widget->grabFramebuffer();
    QString frameFilePath =
        framesDir + QString("/frame%1.bmp").arg(i, 4, 10, QLatin1Char('0'));
    frame.save(frameFilePath, "BMP");
    QCoreApplication::processEvents();
    QThread::msleep(100);
  }
  QStringList ffmpegArgs;
  ffmpegArgs << "-y" << "-framerate" << "10" << "-i"
             << QString("%1/frame%04d.bmp").arg(framesDir) << "-vf"
             << "scale=640:480" << "-loop" << "0" << gifFilePath;
  QProcess ffmpegProcess;
  ffmpegProcess.setProcessChannelMode(QProcess::MergedChannels);
  ffmpegProcess.start("ffmpeg", ffmpegArgs);
  if (!ffmpegProcess.waitForStarted()) {
    QMessageBox::critical(
        this, "Error",
        "Failed to start ffmpeg process: " + ffmpegProcess.errorString());
  }
  ffmpegProcess.waitForFinished();
  if (ffmpegProcess.exitCode() != 0) {
    QString errorOutput = ffmpegProcess.readAllStandardError();
    QMessageBox::critical(this, "Error", "FFmpeg failed:\n" + errorOutput);
    return;
  }
  QMessageBox::information(this, "Success", "GIF saved successfully.");
  QDir(framesDir).removeRecursively();
}

/**
 * @brief Открывает файл модели и загружает его в приложение.
 *
 * @param Нет параметров.
 * @details Вызывает диалог выбора файла, загружает объект в контроллер
 * и обновляет информацию о модели.
 */
void MainWindow::fileOpenButton() {
  QString file_name = QFileDialog::getOpenFileName(this, "Choose OBJ File", "",
                                                   "OBJ Files (*.obj)");
  viewer_controller->Model_loadOBJ(file_name);
  label->setText(QString("file:\n%1\n\nvertices:\n%2\n\nfacets:\n%3")
                     .arg(file_name)
                     .arg(viewer_controller->modelGetVertices().size())
                     .arg(viewer_controller->modelGetFacets().size()));
  openGL_widget->update();
};

/**
 * @brief Класс команды перемещения модели.
 *
 * @param controller Контроллер модели.
 * @param value Величина сдвига.
 * @param translateAction Направление сдвига.
 * @details Выполняет перемещение модели по выбранной оси на заданное
 * расстояние.
 */
TranslateCommand::TranslateCommand(ViewerController *controller, float value,
                                   translateAction_t translateAction)
    : controller_(controller), value(value), trAction(translateAction) {}
void TranslateCommand::execute() {
  controller_->modelTranslateFigure(trAction, value);
}

/**
 * @brief Класс команды вращения модели.
 *
 * @param controller Контроллер модели.
 * @param value Угол поворота.
 * @param rotateAction Ось вращения.
 * @details Выполняет вращение модели вокруг указанной оси.
 */
RotateCommand::RotateCommand(ViewerController *controller, float value,
                             rotateAction_t rotateAction)
    : controller_(controller), value(value), rtAction(rotateAction) {}
void RotateCommand::execute() { controller_->modelRotateAxis(rtAction, value); }

/**
 * @brief Класс команды масштабирования модели.
 *
 * @param controller Контроллер модели.
 * @param scaleValue Коэффициент масштабирования.
 * @param scaleF Тип масштабирования.
 * @details Изменяет размер модели в соответствии с указанным коэффициентом.
 */
ScaleCommand::ScaleCommand(ViewerController *controller, float scaleValue,
                           const ScaleType_t &scaleF)
    : controller_(controller), sclValue(scaleValue), sclF(scaleF) {}
void ScaleCommand::execute() { controller_->modelScaleFigure(sclF, sclValue); }

/**
 * @brief Класс команды изменения цвета граней модели.
 *
 * @param controller Контроллер модели.
 * @param color Новый цвет граней.
 * @details Устанавливает указанный цвет для граней модели.
 */
ChangeFacetColorCommand::ChangeFacetColorCommand(ViewerController *controller,
                                                 QColor color)
    : controller_(controller), color_(color) {}
void ChangeFacetColorCommand::execute() {
  controller_->modelSetFacetColor(color_);
}

/**
 * @brief Класс команды изменения толщины граней модели.
 *
 * @param controller Контроллер модели.
 * @param scaleType Тип изменения толщины.
 * @details Меняет толщину граней в зависимости от переданного параметра.
 */
ChangeFacetWidthCommand::ChangeFacetWidthCommand(ViewerController *controller,
                                                 ScaleType_t scaleType)
    : controller_(controller), scaleType_(scaleType) {}
void ChangeFacetWidthCommand::execute() {
  controller_->modelSetFacetWidth(scaleType_);
}

/**
 * @brief Устанавливает цвет вершин модели.
 *
 * @param controller Контроллер представления.
 * @param color Новый цвет вершин.
 */
ChangeVerticeColorCommand::ChangeVerticeColorCommand(
    ViewerController *controller, QColor color)
    : controller_(controller), color_(color) {}
void ChangeVerticeColorCommand::execute() {
  controller_->modelSetVerticeColor(color_);
}

/**
 * @brief Выполняет команду изменения толщины вершин.
 */
ChangeVerticeWidthCommand::ChangeVerticeWidthCommand(
    ViewerController *controller, ScaleType_t scaleType)
    : controller_(controller), scaleType_(scaleType) {}
void ChangeVerticeWidthCommand::execute() {
  controller_->modelSetVerticeWidth(scaleType_);
}

/**
 * @brief Выполняет команду трансформации (перемещения, вращения,
 * масштабирования).
 *
 * @tparam CommandType Тип команды.
 * @tparam DirectionType Тип направления трансформации.
 * @param input Поле ввода с параметром трансформации.
 * @param direction Направление трансформации.
 */
template <typename CommandType, typename DirectionType>
void MainWindow::executeTransformCommand(QLineEdit *input,
                                         DirectionType direction) {
  QString inputText = input->text();
  auto command = std::make_unique<CommandType>(
      viewer_controller, viewer_controller->modelMakeFloat(inputText),
      direction);
  command->execute();
  openGL_widget->update();
}

/**
 * @brief Выполняет команду перемещения модели.
 *
 * @param input Поле ввода значения смещения.
 * @param translateAction Направление перемещения.
 */
void MainWindow::translate(QLineEdit *input,
                           translateAction_t translateAction) {
  executeTransformCommand<TranslateCommand>(input, translateAction);
}

/**
 * @brief Выполняет команду вращения модели.
 *
 * @param input Поле ввода угла поворота.
 * @param rotateAction Направление вращения.
 */
void MainWindow::rotate(QLineEdit *input, rotateAction_t rotateAction) {
  executeTransformCommand<RotateCommand>(input, rotateAction);
}

/**
 * @brief Выполняет команду масштабирования модели.
 *
 * @param input Поле ввода коэффициента масштабирования.
 * @param scaleType Тип масштабирования.
 */
void MainWindow::scale(QLineEdit *input, ScaleType_t scaleType) {
  executeTransformCommand<ScaleCommand>(input, scaleType);
}

/**
 * @brief Выполняет переданную команду и обновляет OpenGL-виджет.
 *
 * @param command Умный указатель на команду.
 */
void MainWindow::executeCommand(std::unique_ptr<Command> command) {
  command->execute();
  openGL_widget->update();
}

/**
 * @brief Открывает диалоговое окно для выбора цвета и устанавливает цвет рёбер.
 */
void MainWindow::changeFacetColor() {
  QColor color = QColorDialog::getColor(currentFacetColor, this, "Facet Color");
  if (color.isValid()) {
    auto command =
        std::make_unique<ChangeFacetColorCommand>(viewer_controller, color);
    executeCommand(std::move(command));
  }
}

/**
 * @brief Устанавливает толщину рёбер.
 *
 * @param scaleType Тип масштабирования толщины рёбер.
 */
void MainWindow::changeFacetWidth(ScaleType_t scaleType) {
  auto command =
      std::make_unique<ChangeFacetWidthCommand>(viewer_controller, scaleType);
  executeCommand(std::move(command));
}

/**
 * @brief Открывает диалоговое окно для выбора цвета и устанавливает цвет
 * вершин.
 */
void MainWindow::changeVerticeColor() {
  QColor color =
      QColorDialog::getColor(currentVerticeColor, this, "Vertice Color");
  if (color.isValid()) {
    auto command =
        std::make_unique<ChangeVerticeColorCommand>(viewer_controller, color);
    executeCommand(std::move(command));
  }
}

/**
 * @brief Устанавливает толщину вершин.
 *
 * @param scaleType Тип масштабирования толщины вершин.
 */
void MainWindow::changeVerticeWidth(ScaleType_t scaleType) {
  auto command =
      std::make_unique<ChangeVerticeWidthCommand>(viewer_controller, scaleType);
  executeCommand(std::move(command));
}

/**
 * @brief Устанавливает тип отображения вершин.
 *
 * @param verticeType Тип вершин.
 */
void MainWindow::typeVertice(const VerticeType_t &verticeType) {
  viewer_controller->modelSetVerticeType(verticeType);
  openGL_widget->update();
}

/**
 * @brief Сбрасывает параметры модели в значения по умолчанию.
 */
void MainWindow::defaultModel() {
  viewer_controller->modelSetDefault(1);
  translateXInput->clear();
  translateYInput->clear();
  translateZInput->clear();
  rotateXInput->clear();
  rotateYInput->clear();
  rotateZInput->clear();
  scaleInput->clear();
  openGL_widget->update();
}

/**
 * @brief Устанавливает тип проекции модели.
 *
 * @param projectionType Тип проекции.
 */
void MainWindow::setProjection(const ProjectionType_t &projectionType) {
  viewer_controller->modelSetProjection(projectionType);
  openGL_widget->update();
}

/**
 * @brief Открывает диалоговое окно для выбора цвета фона и изменяет его.
 */
void MainWindow::changeBackGroundColor() {
  QColor color =
      QColorDialog::getColor(currentBackGroundColor, this, "BackGround Color");
  if (color.isValid()) {
    viewer_controller->modelSetBackGroundColor(color);
    openGL_widget->update();
  }
}

}  // namespace s21