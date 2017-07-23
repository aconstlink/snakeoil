//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_TIMELINE_TIMELINE_H_
#define _SNAKEOIL_QT_TIMELINE_TIMELINE_H_

#include "../typedefs.h"
#include "../protos.h"

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>

namespace so_qt
{
    class timeline : public QWidget
    {
        Q_OBJECT

    private:

        QGraphicsScene * _gs = nullptr ;
        timeline_view * _gv = nullptr ;

    public:

        timeline( ulonglong_t max_ms, so_qt::timer * timer = nullptr ) ;
        timeline( uint_t min, uint_t sec, uint_t mil, so_qt::timer * timer = nullptr ) ;
        virtual ~timeline( void ) ;

    public:

        void connect( timer * timer_ptr ) ;

    protected slots:

        void change_time( unsigned long long ) ;
    };

}

#endif
