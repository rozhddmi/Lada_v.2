#ifndef INDICATORS_H
#define INDICATORS_H


class Indicators
        {
        private:
            int led_row;
            int led_column;
            int bit_num;
            uint8_t* maxRowPointer;
            int onState;
        public:
            Indicators(int bit_num, int row, int column, int pinNum, uint8_t* maxRowPointer , int onState = HIGH);
            ~Indicators();
            void switchOn();
            void switchOff();
            void setState(int coddedVal);

        }

#endif // INDICATORS_H
