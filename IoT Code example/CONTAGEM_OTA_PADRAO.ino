
/*#define HTTPC_ERROR_CONNECTION_REFUSED  (-1)
#define HTTPC_ERROR_SEND_HEADER_FAILED  (-2)
#define HTTPC_ERROR_SEND_PAYLOAD_FAILED (-3)
#define HTTPC_ERROR_NOT_CONNECTED       (-4)
#define HTTPC_ERROR_CONNECTION_LOST     (-5)
#define HTTPC_ERROR_NO_STREAM           (-6)
#define HTTPC_ERROR_NO_HTTP_SERVER      (-7)
#define HTTPC_ERROR_TOO_LESS_RAM        (-8)
#define HTTPC_ERROR_ENCODING            (-9)
#define HTTPC_ERROR_STREAM_WRITE        (-10)
#define HTTPC_ERROR_READ_TIMEOUT        (-11)
    HTTP_CODE_CONTINUE = 100,
    HTTP_CODE_SWITCHING_PROTOCOLS = 101,
    HTTP_CODE_PROCESSING = 102,
    HTTP_CODE_OK = 200,
    HTTP_CODE_CREATED = 201,
    HTTP_CODE_ACCEPTED = 202,
    HTTP_CODE_NON_AUTHORITATIVE_INFORMATION = 203,
    HTTP_CODE_NO_CONTENT = 204,
    HTTP_CODE_RESET_CONTENT = 205,
    HTTP_CODE_PARTIAL_CONTENT = 206,
    HTTP_CODE_MULTI_STATUS = 207,
    HTTP_CODE_ALREADY_REPORTED = 208,
    HTTP_CODE_IM_USED = 226,
    HTTP_CODE_MULTIPLE_CHOICES = 300,
    HTTP_CODE_MOVED_PERMANENTLY = 301,
    HTTP_CODE_FOUND = 302,
    HTTP_CODE_SEE_OTHER = 303,
    HTTP_CODE_NOT_MODIFIED = 304,
    HTTP_CODE_USE_PROXY = 305,
    HTTP_CODE_TEMPORARY_REDIRECT = 307,
    HTTP_CODE_PERMANENT_REDIRECT = 308,
    HTTP_CODE_BAD_REQUEST = 400,
    HTTP_CODE_UNAUTHORIZED = 401,
    HTTP_CODE_PAYMENT_REQUIRED = 402,
    HTTP_CODE_FORBIDDEN = 403,
    HTTP_CODE_NOT_FOUND = 404,
    HTTP_CODE_METHOD_NOT_ALLOWED = 405,
    HTTP_CODE_NOT_ACCEPTABLE = 406,
    HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED = 407,
    HTTP_CODE_REQUEST_TIMEOUT = 408,
    HTTP_CODE_CONFLICT = 409,
    HTTP_CODE_GONE = 410,
    HTTP_CODE_LENGTH_REQUIRED = 411,
    HTTP_CODE_PRECONDITION_FAILED = 412,
    HTTP_CODE_PAYLOAD_TOO_LARGE = 413,
    HTTP_CODE_URI_TOO_LONG = 414,
    HTTP_CODE_UNSUPPORTED_MEDIA_TYPE = 415,
    HTTP_CODE_RANGE_NOT_SATISFIABLE = 416,
    HTTP_CODE_EXPECTATION_FAILED = 417,
    HTTP_CODE_MISDIRECTED_REQUEST = 421,
    HTTP_CODE_UNPROCESSABLE_ENTITY = 422,
    HTTP_CODE_LOCKED = 423,
    HTTP_CODE_FAILED_DEPENDENCY = 424,
    HTTP_CODE_UPGRADE_REQUIRED = 426,
    HTTP_CODE_PRECONDITION_REQUIRED = 428,
    HTTP_CODE_TOO_MANY_REQUESTS = 429,
    HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
    HTTP_CODE_INTERNAL_SERVER_ERROR = 500,
    HTTP_CODE_NOT_IMPLEMENTED = 501,
    HTTP_CODE_BAD_GATEWAY = 502,
    HTTP_CODE_SERVICE_UNAVAILABLE = 503,
    HTTP_CODE_GATEWAY_TIMEOUT = 504,
    HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED = 505,
    HTTP_CODE_VARIANT_ALSO_NEGOTIATES = 506,
    HTTP_CODE_INSUFFICIENT_STORAGE = 507,
    HTTP_CODE_LOOP_DETECTED = 508,
    HTTP_CODE_NOT_EXTENDED = 510,
    HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED = 511

*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h> //lib do ArduinoOTA 
#include <Arduino.h>
#include <HTTPClient.h>
#include <NTPClient.h>//Biblioteca do NTP.
#include <WiFiUdp.h>//para hora mundial
#include "driver/timer.h"
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

#ifdef __cplusplus
}

#endif
const char* host = "M3";
HTTPClient http;

WebServer server(127);

/*
 * Login page
 */

const char* loginIndex = 
 "<form name='loginForm'>"
    "<table width='20%' bgcolor='0000ff' align='center'>"
        "<tr>"
            "<td colspan=2>"
                "<center><font face=arial size=4 color=white><b>StarMeUp OTA</b></font></center>"
                "<br>"
            "</td>"
            "<br>"
            "<br>"
        "</tr>"
        "<td><font size=4 face=arial color=white>Username:</font></td>"
        "<td><input type='text' size=25 name='userid'><br></td>"
        "</tr>"
        "<br>"
        "<br>"
        "<tr>"
            "<td><font size=4 face=arial color=white>Password:</td>"
            "<td><input type='Password' size=25 name='pwd'><br></td>"
            "<br>"
            "<br>"
        "</tr>"
        "<tr>"
            "<td><input type='submit' onclick='check(this.form)' value='Login'></td>"
        "</tr>"
    "</table>"
"</form>"
"<script>"
    "function check(form)"
    "{"
    "if(form.userid.value=='admin' && form.pwd.value=='admin')"
    "{"
    "window.open('/serverIndex')"
    "}"
    "else"
    "{"
    " alert('Error Password or Username')/*displays error message*/"
    "}"
    "}"
"</script>";
 
/*
 * Server Index Page
 */
 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

uint8_t temp_farenheit;
float temp_celsius;



WiFiUDP udp;//Cria um objeto "UDP".
NTPClient ntp(udp, "a.st1.ntp.br", -3*3600,60000);//Cria um objeto "NTP" com as configurações -3
int d,m,y,h,mi,s;

char* ssid     = "ssid";
char* password = "password";



int cont=0;
int sensor=2; //2 verificar
int vsensor=0;
int tempo_prod=0;
int tempo_turno=0;
int ultimo=0;
int hitObject = false;
int hitObject2 = false;
String payload;
String url;
int httpCode;
int addTempo; // para continuar o tempo
String  hora; 
String token = "channel token"; 
String mc="machine code";// máquina
int ant=0;



//timers
static intr_handle_t s_timer_handle;
void inicia_timer0()
{
 timer_config_t config = {
                          .alarm_en = true,
                          .counter_en = false,
                          .intr_type = TIMER_INTR_LEVEL,
                          .counter_dir = TIMER_COUNT_UP,
                          .auto_reload = true,
                          .divider = 80   /* 1 us per tick */
                          };
    
 timer_init(TIMER_GROUP_0, TIMER_0, &config);  
}

void start_timer0()
{
  timer_start(TIMER_GROUP_0, TIMER_0);
}

void pausa_timer0()
{
 timer_pause(TIMER_GROUP_0,TIMER_0);
}

double get_tempo_sec_timer0()
{
 double tp;
 timer_get_counter_time_sec(TIMER_GROUP_0,TIMER_0, &tp);
 return tp*2;//ajuste segundos
}
          
void reseta_timer0(uint64_t valor_reset)
{
 timer_set_counter_value(TIMER_GROUP_0, TIMER_0, valor_reset);
}

void inicia_timer1()
 {
  timer_config_t config = {
                           .alarm_en = true,
                           .counter_en = false,
                           .intr_type = TIMER_INTR_LEVEL,
                           .counter_dir = TIMER_COUNT_UP,
                           .auto_reload = true,
                           .divider = 80   /* 1 us per tick */
                          };
    
  timer_init(TIMER_GROUP_0, TIMER_1, &config);  
}

void start_timer1()
{
 timer_start(TIMER_GROUP_0, TIMER_1);
}

void pausa_timer1()
{
 timer_pause(TIMER_GROUP_0,TIMER_1);
}

double get_tempo_sec_timer1()
{
  double tp;
  timer_get_counter_time_sec(TIMER_GROUP_0,TIMER_1, &tp);
  return tp*2;//ajuste segundos
}
          
void reseta_timer1(uint64_t valor_reset)
{
 timer_set_counter_value(TIMER_GROUP_0, TIMER_1, valor_reset);
}

void ini_cont(){
  url = "http://oee-server.appspot.com/api/"+mc+"/getMax?token="+token+"&fields=5";
  http.begin(url);
  http.setReuse(true);
  httpCode =http.GET(); 
  payload = http.getString();
  addTempo = payload.toInt();
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
 
  url = "http://oee-server.appspot.com/api/"+mc+"/getMax?token="+token+"&fields=3";
  http.begin(url);
  http.setReuse(true);
  httpCode =http.GET(); 
  payload = http.getString();
  int maxC = payload.toInt();
  cont=maxC;
  ant=cont;
  http.end();
  
}


void setup() {
 
 Serial.begin(115200);
 pinMode(sensor,INPUT);

 WiFi.begin(ssid, password);
 Serial.println("");
 int creset=0;
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(creset);
    creset++;
    if (creset>=40)
    {
    break;
    }
    
  }
  
  
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
   
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://M3.local
   
    while (1) {
      delay(300);
    }
  }
  ini_cont();
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
     
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
 
 server.begin();
 inicia_timer0;//Incializa Timer0
 reseta_timer0(0);// incializa com 0
 inicia_timer1;//Incializa Timer0
 reseta_timer1(0);// incializa com 0
 start_timer1();// timer do tempo turno 

}

void send_api(String token, String mc, String f1, int f2, int f3, float f4, int f5)
     {
     url = "http://oee-server.appspot.com/api/feed/update?token="+token+"&mc_cd=" + mc +"&field1=" + f1+"&field2=" + f2 +"&field5=" + String(f5) + "&field3="+ String(f3)+"&field4="+ String(f4);
     http.begin(url);
     http.setReuse(true);
     httpCode=http.GET();
     http.end(); 
     }

void loop(){
   server.handleClient();
   vsensor= digitalRead(sensor); // le o sensor primeira coisa do loop
   

   temp_farenheit= temprature_sens_read();
   temp_celsius = ( temp_farenheit - 32 ) / 1.8;
   if(WiFi.status() == WL_CONNECTED)
   {
      ntp.begin();//instancia objeto ntp
      ntp.forceUpdate(); // força update
      unsigned long epochTime =  ntp.getEpochTime(); // converte em temp
      hora= ntp.getFormattedTime();//Armazena na váriavel HORA, o horario atual. // converte em horas
      
      ntp.end(); // encerra objeto, limpa da memória
      // convert received time stamp to time_t object
      time_t t ;
      t = epochTime;
      struct tm timeinfo;
      localtime_r(&t, &timeinfo);
      char buffer[100];
      // es. 08/05/2017 15:10:34
      strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
      d=timeinfo.tm_mday;
      m=timeinfo.tm_mon;
      y=timeinfo.tm_year;
      h=timeinfo.tm_hour;//horário de verão
      mi=timeinfo.tm_min;
      s=timeinfo.tm_sec;
      h=h+1;
     
      if(h==5&&mi==20 && s==00 || h==14&&mi==53 && s==00  )
        {   
          cont=0;// garante que vai zerar o contador apos o idle mode
          tempo_turno=0;
          tempo_prod=0;
          addTempo=0;

        }
          if(h>=23&&mi==46 && s==00 || h<=5  )
        {   
          cont=0;// garante que vai zerar o contador apos o idle mode
          tempo_turno=0;
          tempo_prod=0;
          addTempo=0;  
        }else{    
         start_timer0(); // inicia turno 
         tempo_turno = int(round(get_tempo_sec_timer1()/60));
         
          if( (vsensor == HIGH)){
               start_timer0(); // mede tempo produtivo
                tempo_prod= int(round(get_tempo_sec_timer0()/60));
                Serial.println(tempo_turno);
                Serial.println("");
                Serial.println(tempo_prod);
                Serial.println("");
                Serial.print("OEE");
                Serial.println(int(tempo_prod/tempo_turno)*100);
              }
        
            }//else
          if(tempo_turno >0 && tempo_turno % 1 == 0 && ultimo < tempo_turno )
              {  
              ultimo=tempo_turno;
              tempo_prod=int(round(get_tempo_sec_timer0()/60));
              send_api(token,mc,String(int(tempo_prod/tempo_turno)),httpCode,tempo_prod,temp_celsius,tempo_turno);//send_api(String token,String mc, String f1, int f2, int f3, float f4, int f5)
            
             // Serial.println(hora);
             
           }
         }//wifi   
       }
       
       
       //loop
