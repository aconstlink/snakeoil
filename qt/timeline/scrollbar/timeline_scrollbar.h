//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_QT_TIMELINE_SCROLLBAR_H_
#define _SNAKEOIL_QT_TIMELINE_SCROLLBAR_H_

#include <QScrollBar>

namespace so_qt
{
    class timeline_scrollbar : public QScrollBar
    {
        Q_OBJECT

    public:

        timeline_scrollbar( void ) ;
        virtual ~timeline_scrollbar( void ) ;

        virtual void paintEvent( QPaintEvent * event ) ;
    };
}

#endif
