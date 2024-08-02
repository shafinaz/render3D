#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <qDebug>
/*
 * Class : graph
 * Member : graph
 * Description : This method is a constructor for class graph.
 */
graph::graph(QGraphicsView *view, QWidget *parent): QGraphicsView(parent), scene(new QGraphicsScene(this))
{

    QRectF viewRect = view->rect();
    view->setScene(scene);
    scene->setSceneRect(-viewRect.width() / 2.0, -viewRect.height() / 2.0, viewRect.width(), viewRect.height());
    QTransform transform;
    transform.scale(1, -1); // Invert the y-axis
    view->setTransform(transform);
    drawAxis();
}

/*
 * Class : graph
 * Member : drawAxis
 * Description : This method plots out the axis on the scene.
 */
void graph::drawAxis()
{
    qreal halfW = scene->sceneRect().width() / 2.0;
    qreal halfH = scene->sceneRect().height() / 2.0;

    QGraphicsLineItem *xAxis = new QGraphicsLineItem(-halfW, 0, halfW, 0);  // X axis
    QGraphicsLineItem *yAxis = new QGraphicsLineItem(0, -halfH, 0, halfH);  // Y axis

    qreal intervalX = scene->sceneRect().width() / 10;
    qreal intervalY = scene->sceneRect().height() / 10;

    QPen axisPen(Qt::white);
    xAxis->setPen(axisPen);
    yAxis->setPen(axisPen);

    QPen scalePen(Qt::white);
    QFont labelFont("Arial", 10);

    scene->addItem(xAxis);
    scene->addItem(yAxis);

    for(qreal x = -halfW; x <= halfW; x += intervalX)
    {
        //add scale marks
        QGraphicsLineItem *xScale = new QGraphicsLineItem(x, -5, x, 5);
        xScale->setPen(scalePen);
        scene->addItem(xScale);

        // Calculate label value for range -5 to 5
        qreal labelValue = (x / intervalX) * 5.0 / (halfW / intervalX);

        //create labels
        QGraphicsTextItem *xLabel = new QGraphicsTextItem(QString::number(labelValue));
        xLabel->setFont(labelFont);
        xLabel->setPos(x, 5);

        //invert
        QTransform inverseTransform;
        inverseTransform.scale(1, -1);
        xLabel->setTransform(inverseTransform);

        scene->addItem(xLabel);

    }

    for(qreal y = -halfH; y <= halfH; y += intervalY)
    {
        //add scale marks
        QGraphicsLineItem *yScale = new QGraphicsLineItem(-5, y, 5, y);
        yScale->setPen(scalePen);
        scene->addItem(yScale);

        // Calculate label value for range -5 to 5
        qreal labelValue = (y / intervalY) * 5.0 / (halfH / intervalY);

        //create labels
        QGraphicsTextItem *yLabel = new QGraphicsTextItem(QString::number(labelValue));
        yLabel->setFont(labelFont);
        yLabel->setPos(5, y);

        //invert
        QTransform inverseTransform;
        inverseTransform.scale(1, -1);
        yLabel->setTransform(inverseTransform);

        scene->addItem(yLabel);

    }

}

/*
 * Class : graph
 * Member : vertices
 * Description : This method returns the saved list of vertices.
 */
QList<vertex*> graph::vertices() const
{
    return vertexList;
}

/*
 * Class : graph
 * Member : edges
 * Description : This method returns the saved list of edges.
 */
QList<edge*> graph::edges() const
{
    return edgeList;
}

/*
 * Class : graph
 * Member : addVertex
 * Description : This method adds a vertex in the list.
 */
void graph::addVertex(int x, int y)
{
    vertex *v = new vertex(x, y);
    vertexList.append(v);
    scene->addItem(v);
}

/*
 * Class : graph
 * Member : addEdge
 * Description : This method adds an edge in the list.
 */
void graph::addEdge(vertex *v1, vertex *v2)
{
    edge *e = new edge(v1, v2);
    edgeList.append(e);
    scene->addItem(e);
}

/*
 * Class : vertex
 * Member : vertex
 * Description : This method is a constructor for class vertex.
 */
vertex::vertex(qreal x, qreal y): QGraphicsEllipseItem(x, y, 10, 5)
{
    setBrush(Qt::white); // Set brush color
    setPen(QPen(Qt::black)); // Set outline pen
    newPos = QPointF(x, y);
    setZValue(-1);
}

/*
 * Class : vertex
 * Member : retPos
 * Description : This method returns the position of a defined vertex.
 */
QPointF vertex::retPos() const
{
    return newPos;
}

/*
 * Class : edge
 * Member : edge
 * Description : This method is a constructor for class edge.
 */
edge::edge(vertex *sourceVx, vertex *destVx)
    : QGraphicsLineItem()
{
    if (!sourceVx || !destVx)
        return;

    setLine(sourceVx->retPos().x(), sourceVx->retPos().y(), destVx->retPos().x(), destVx->retPos().y());

}

/*
 * Class : edge
 * Member : paint
 * Description : This method sets the attributes of the edge line.
 */
void edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPen pen(Qt::white);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->drawLine(line());
}

void MainWindow::parseGraphFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();

    // Read total vertices and edges
    QStringList totals = line.split(',');
    int totalVertices = totals[0].toInt();
    int totalEdges = totals[1].toInt();

    QMap<int, vertex*> verticesMap; //each vertex will be indexed

    // Read vertex information
    for (int i = 0; i < totalVertices; ++i) {
        line = in.readLine();
        QStringList vertexData = line.split(',');
        int index = vertexData[0].toInt();
        float x = vertexData[1].toFloat();
        float y = vertexData[2].toFloat();

        x *= scaleX; //value of x * 640 (pixel) / 10 (scale)
        y *= scaleY;

        vertex *v = new vertex(x, y);
        verticesMap.insert(index, v);
        graphView->addVertex(x, y);
    }

    // Read edge information
    for (int i = 0; i < totalEdges; ++i) {
        line = in.readLine();
        QStringList edgeData = line.split(',');
        int vertexIndex1 = edgeData[0].toInt();
        int vertexIndex2 = edgeData[1].toInt();

        vertex *v1 = verticesMap.value(vertexIndex1);
        vertex *v2 = verticesMap.value(vertexIndex2);

        graphView->addEdge(v1, v2);
    }

    file.close();
}

/*
 * Class : MainWindow
 * Member : MainWindow
 * Description : This method is a constructor for class MainWindow.
 */
MainWindow::MainWindow(QWidget *parent, const QString &filename) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QGraphicsView *uiGraphicsView = ui->graphicsView;

    graphView = new graph(uiGraphicsView);

    // Parse graph data from file if filename is provided
    if (!filename.isEmpty()) {
        parseGraphFile(filename);
    }

}

/*
 * Class : MainWindow
 * Member : MainWindow
 * Description : This method is a destructor for class MainWindow.
 */
MainWindow::~MainWindow()
{
    delete ui;
}

