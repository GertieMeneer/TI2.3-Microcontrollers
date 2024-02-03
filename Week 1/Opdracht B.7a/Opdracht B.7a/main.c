#include <avr/io.h>
#include <stdio.h>

enum States {
    START,
    S1,
    S2,
    S3,
    END
};

void start(void);
void s1(void);
void s2(void);
void s3(void);
void end(void);

void start(void) {
    printf("start\n");
}

void s1(void) {
    printf("s1\n");
}

void s2(void) {
    printf("s2\n");
}

void s3(void) {
    printf("s3\n");
}

void end(void) {
    printf("s4\n");
}

// *currentState inside the function, it is dereferencing the pointer to access or modify the actual value of currentState.
void transitionState(enum States *currentState, uint8_t input) {
    switch (*currentState) {
        case START:
            if (input & (1 << PIND5) || input & (1 << PIND6)) {
                *currentState = S2;
            }
            break;
        case S1:
            if (input & (1 << PIND5)) {
                *currentState = S2;
            } else if (input & (1 << PIND7)) {
                *currentState = START;
            }
            break;
        case S2:
            if (input & (1 << PIND7)) {
                *currentState = START;
            } else if (input & (1 << PIND6)) {
                *currentState = S1;
            } else if (input & (1 << PIND5)) {
                *currentState = S3;
            }
            break;
        case S3:
            if (input & (1 << PIND7)) {
                *currentState = START;
            } else if (input & ((1 << PIND5) | (1 << PIND6))) {
                *currentState = END;
            }
            break;
        case END:
            if (input & (1 << PIND7)) {
                *currentState = START;
            }
            break;
    }
}

int main(void) {
    enum States currentState = START;

    while (1) {
        // Read input from PINx.5, PINx.6, PINx.7
        uint8_t input = PIND & ((1 << PIND5) | (1 << PIND6) | (1 << PIND7));

		// Passing the address of currentState to the transitionState function, allowing it to modify the value of currentState directly.
        transitionState(&currentState, input);

        switch (currentState) {
            case START:
                start();
                break;
            case S1:
                s1();
                break;
            case S2:
                s2();
                break;
            case S3:
                s3();
                break;
            case END:
                end();
                break;
        }
    }

    return 0;
}
