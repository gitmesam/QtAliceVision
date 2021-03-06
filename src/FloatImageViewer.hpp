#pragma once

#include "FloatTexture.hpp"

#include <QQuickItem>
#include <QUrl>
#include <QRunnable>
#include <QSharedPointer>
#include <QVariant>
#include <QVector4D>

#include <memory>


namespace qtAliceVision
{

/**
 * @brief QRunnable object dedicated to load image using AliceVision.
 */
class FloatImageIORunnable : public QObject, public QRunnable
{
    Q_OBJECT

public:

    explicit FloatImageIORunnable(const QUrl & path, QObject * parent = nullptr);

    /// Load image at path
    Q_SLOT void run() override;

    /// Emitted when the image is loaded
    Q_SIGNAL void resultReady(QSharedPointer<qtAliceVision::FloatImage> image, QSize sourceSize, const QVariantMap & metadata);
    
private:
    QUrl _path;
};

/**
 * @brief Load and display image.
 */
class FloatImageViewer : public QQuickItem
{
    Q_OBJECT
    /// Path to image
    Q_PROPERTY(QUrl source MEMBER _source NOTIFY sourceChanged)
    /// 
    Q_PROPERTY(float gamma MEMBER _gamma NOTIFY gammaChanged)
    /// 
    Q_PROPERTY(float offset MEMBER _offset NOTIFY offsetChanged)
    ///
    Q_PROPERTY(QSize textureSize MEMBER _textureSize NOTIFY textureSizeChanged)
    ///
    Q_PROPERTY(QSize sourceSize READ sourceSize NOTIFY sourceSizeChanged)
    /// Whether the image is currently being loaded from file
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    /// Whether to clear image between two loadings
    Q_PROPERTY(bool clearBeforeLoad MEMBER _clearBeforeLoad NOTIFY clearBeforeLoadChanged)

    Q_PROPERTY(EChannelMode channelMode MEMBER _channelMode NOTIFY channelModeChanged)
    
    Q_PROPERTY(QVariantMap metadata READ metadata NOTIFY metadataChanged)

public:
    explicit FloatImageViewer(QQuickItem* parent = nullptr);
    ~FloatImageViewer() override;

    bool loading() const
    {
        return _loading;
    }

    void setLoading(bool loading);

    QSize sourceSize() const
    {
        return _sourceSize;
    }

    const QVariantMap & metadata() const
    {
        return _metadata;
    }

    enum class EChannelMode : quint8 { RGBA, RGB, R, G, B, A };
    Q_ENUM(EChannelMode)

public:
    Q_SIGNAL void sourceChanged();
    Q_SIGNAL void loadingChanged();
    Q_SIGNAL void clearBeforeLoadChanged();
    Q_SIGNAL void gammaChanged();
    Q_SIGNAL void offsetChanged();
    Q_SIGNAL void textureSizeChanged();
    Q_SIGNAL void sourceSizeChanged();
    Q_SIGNAL void channelModeChanged();
    Q_SIGNAL void imageChanged();
    Q_SIGNAL void metadataChanged();

    Q_INVOKABLE QVector4D pixelValueAt(int x, int y);

private:
    /// Reload image from source
    void reload();
    /// Handle result from asynchronous file loading
    Q_SLOT void onResultReady(QSharedPointer<qtAliceVision::FloatImage> image, QSize sourceSize, const QVariantMap & metadata);
    /// Custom QSGNode update
    QSGNode* updatePaintNode(QSGNode* oldNode, QQuickItem::UpdatePaintNodeData* data) override;

private:
    QUrl _source;
    float _gamma = 1.f;
    float _offset = 0.f;

    bool _loading = false;
    bool _outdated = false;
    bool _clearBeforeLoad = true;

    bool _imageChanged = false;
    EChannelMode _channelMode;
    QSharedPointer<FloatImage> _image;
    QRectF _boundingRect;
    QSize _textureSize;
    QSize _sourceSize = QSize(0,0);

    QVariantMap _metadata;
};

}  // ns qtAliceVision

Q_DECLARE_METATYPE(qtAliceVision::FloatImage);
Q_DECLARE_METATYPE(QSharedPointer<qtAliceVision::FloatImage>);
