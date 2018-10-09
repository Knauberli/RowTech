#include "test.h"



test::test()
{
    void super();

}

void super()
    {
            QFile file("test.txt");

            if(!file.exists())
            {
                qDebug() << file.fileName() << " does not exist";
            }
            if(file.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream txtStream(&file);

                qDebug() << "--- Writing to file ----";

                txtStream << "The first line\n";
                txtStream << "Next line";

                qDebug() << "--- Reading from file ----";


                txtStream.seek(0);
                file.close();
            }
            else
            {
                qDebug() << "Could not open the file";
                return;
            }
     }
