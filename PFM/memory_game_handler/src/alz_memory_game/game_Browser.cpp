#include "game_classes.h"

/*********************************************************************************
********************************alzBrowserGameClass*******************************
*********************************************************************************/
void alzBrowserGameClass::closeBrowser(){
  cout << "BROWSER -- CLOSE BROWSER\n";
  std::stringstream closeBrowser;
  closeBrowser << "pkill -f chromium-browser";
  system(closeBrowser.str().c_str());
}
void alzBrowserGameClass::sendFinishGame(){
  memory_game_handler::Feedback feedback_msg;
  feedback_msg.game_name=game_name;
  feedback_msg.game_status=STOPPED;
  this->internalFeedback_pub.publish(feedback_msg);
  alzBrowserGameClass::closeBrowser();
}
void alzBrowserGameClass::speakIntro(string package){
  //Read the .xml document

  XmlDocument doc_games;
  std::string pathPKG = ros::package::getPath(package);
  char pathXML[200];
  sprintf(pathXML,"%s/data/%s.xml", pathPKG.c_str(), game_name.c_str());
  doc_games.load_from_file(pathXML);

  std::vector<XmlDocument::Node*> nodes_game;
  string str_game_option=game_name+".item";
  doc_games.get_all_nodes_at_direction(doc_games.root(),str_game_option,nodes_game);
  printf("#objects = %i\n", nodes_game.size());

  for (unsigned int i = 0; i < nodes_game.size(); ++i)
  {

    XmlDocument::Node * n_obj = nodes_game.at(i);
    string n_sentence=doc_games.get_value(n_obj,"sentence");
    string n_gesture_init=doc_games.get_value(n_obj,"gesture_init");
    string n_wait_time_str=doc_games.get_value(n_obj,"wait_time");
    int n_wait_time=atoi(n_wait_time_str.c_str());
    cout << "SPEAKING: " << n_sentence.c_str() << "\n";
    cout << "wait: "<< n_wait_time_str << "\n";
    this->makeGesture(n_gesture_init);
    saySentence(n_sentence.c_str(),QUEUE,n_wait_time);
 
    //sleep(n_wait_time);
  }

}
string alzBrowserGameClass::getRandomNumberString(string number){
  string numberPhrase = "";
  if(number=="one")
    numberPhrase =  "Giremos la carta uno! | Carta uno! | Primera carta!";
  else if(number=="two")
    numberPhrase =  "Giremos la carta dos! | Carta dos! | Segunda carta!";
  else if(number=="three")
    numberPhrase =  "Giremos la carta tres! | Carta tres! | Tercera carta!";
  else if(number=="four")
    numberPhrase =  "Giremos la carta cuatro! | Carta cuatro! | Cuarta carta!";
  else if(number=="five")
    numberPhrase =  "Giremos la carta cinco! | Carta cinco! | Quinta carta!";
  else if(number=="six")
    numberPhrase =  "Giremos la carta seis! | Carta seis! | Sexta carta!";
  else if(number=="seven")
    numberPhrase =  "Giremos la carta siete! | Carta siete! | Séptima carta!";
  else if(number=="eight")
    numberPhrase =  "Giremos la carta ocho! | Carta ocho! | Octava carta!";
  else if(number=="nine")
    numberPhrase =  "Giremos la carta nueve! | Carta nueve! | Novena carta!";
  else if(number=="ten")
    numberPhrase =  "Giremos la carta diez! | Carta diez! | Décima carta!";
  else if(number=="eleven")
    numberPhrase =  "Giremos la carta once! | Carta once! | Undécima carta!";
  else if(number=="twelve")
    numberPhrase =  "Giremos la carta doce! | Carta doce! | Duodécima carta!";
  return getRandomString(numberPhrase);
}
string alzBrowserGameClass::getRandomString(string sentencesToSynthesize){
  std::vector<std::string> snippets;
  StringUtils::StringSplit(sentencesToSynthesize, "|", &snippets);
  //select one sentence randomly
  srand (time(NULL));
  int rnd_idx = rand() % snippets.size();
  return snippets[rnd_idx];
}
void alzBrowserGameClass::openGameInBrowser(){
  cout << "BROWSER -- OPEN BROWSER\n";
  std::stringstream openInBrowser;
  openInBrowser << "chromium-browser file:///home/user/catkin_ws/src/memory_game_handler/games%20BROWSER/" << this->game_name << ".html &";
  system(openInBrowser.str().c_str());
}
void alzBrowserGameClass::dialogCallback_memGame(const std_msgs::String::ConstPtr& msg){
  cout << "BROWSER -- DIALOG CALLBACK\n";
  cout << "I receive from alz/reportCommandResult " << msg->data << "\n";
  if(msg->data == "t"){
    //User found a pair of cards
    saySentence(getRandomString("Bien hecho! | Genial! | Ya queda poco! | Eso es! | Muy bien!"),SHUTUP,0);
  }else if(msg->data=="f"){
    //Cards selected are not a pair
    saySentence(getRandomString("No, no es correcto. Prueba otra vez! | No. Probemos otra vez! | Esa no! Probemos de nuevo! | No es correcto, pero no pasa nada, sigamos probando"),SHUTUP,0);
  }else if(msg->data=="a"){
    //User found all the pairs
    saySentence(getRandomString("Enhorabuena! Hemos acabado! | A qué se nos ha dado genial? | Qué buenos jugadores!"),SHUTUP,0);
    sleep(5);
    this->game_status=STOPPED;
    sendFinishGame();
  }
}
void alzBrowserGameClass::makeGesture(string gesture){
  std_msgs::String gesture_to_send;
  gesture_to_send.data = gesture;
  make_gesture.publish(gesture_to_send);
}
void alzBrowserGameClass::saySentence(string sentence, int priority, int waitTime){
  etts_msgs::Utterance etts_msg;
  etts_msg.emotion = etts_msg.EMOTION_HAPPY;
  etts_msg.text =sentence;
  switch(priority){
    case QUEUE:
      etts_msg.priority = etts_msgs::Utterance::QUEUE_SENTENCE;
      break;
    case SHUTUP:
      etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
      break;
    default:
      etts_msg.priority = etts_msgs::Utterance::SHUTUP_IMMEDIATLY_AND_SAY_SENTENCE;
  }      
 etts_say_text.publish(etts_msg);
 if(waitTime==0) sleep(5);
 else sleep(waitTime);
}
void alzBrowserGameClass::sendGameCommand(string command){
  cout << "BROWSER -- SEND GAME COMMAND\n";
  // Publish alz/ask4GameCommand --> we publish on this topic the number of the card to show
  // cardsRequestHandler will be listening to this topic, in order to tell the browser to show that card
  std_msgs::String command_to_send;
  command_to_send.data = command;
  std::cout << "PUBLICANDO alz/ask4GameCommand con valor " << command << "\n";
  ask4GameCommand_memGame_pub.publish(command_to_send);
}
bool alzBrowserGameClass::isANumber(string msg){
  cout << "BROWSER -- ISNUMBER\n";
    int i=0; bool found=false;
    string numbers[] = {"one","two","three","four","five","six","seven","eight","nine","ten","eleven","twelve"};
    //if(gameStarted){
      while(!found && i<numCards){
          if(msg==numbers[i])
              found=true;
          else
              i++;
      }
    //}
    return found;
}
void alzBrowserGameClass::ask4Hint(){
  cout << "BROWSER -- HINT\n";
  // We can use the communication channel between browser and robot to ask for a hint
  this->sendGameCommand("hint");
}
void alzBrowserGameClass::setLevelGame(string level){
  cout << "BROWSER -- LEVEL\n";
  string levelPhrase="";
  if(level!=""){
    this->game_level = level;
    if(level=="easy_level"){
        levelPhrase="Empecemos con algo sencillo | Vamos a jugar con el nivel fácil";
        this->numCards=4; //4 cards --> we check to the third position in the array
    }else if(level=="medium_level"){
        levelPhrase="Vale, ni muy fácil ni muy difícil | Estupendo! Nivel medio";
        this->numCards=8; //8 cards --> we check to the seventh position in the array
    }else if(level=="hard_level"){
        levelPhrase="\\emphasis Guau!! Eres todo un valiente! | Vale! Me gusta el nivel difícil! | Ya verás como no es tan complicado";
        this->numCards=12; //12 cards --> we check to the eleventh position in the array
    }
    cout << "Level of game established: " << level << "\n";
    this->sendGameCommand(level);
    saySentence(getRandomString(levelPhrase),SHUTUP,0);
  }   
}
bool alzBrowserGameClass::isLevelCommand(string msg){
  return (msg=="easy_level" || msg=="medium_level" || msg=="hard_level");
}
bool alzBrowserGameClass::isAGameCommand(string msg){
  cout << "BROWSER -- ISGAMECOMMAND\n";
    return this->isLevelCommand(msg)  
        || (this->game_name=="memory_game" && (this->isANumber(msg) || msg=="get_hint" || msg=="restart"))
        || (this->game_name=="differences_game" && (!this->isANumber(msg) && msg!="get_hint"));
}
void alzBrowserGameClass::handleGame(const memory_game_handler::Command& gameCommand){
  cout << "BROWSER ME MANDAN UN COMANDO INTERNO = "<<gameCommand.command <<"\n";
  if(gameCommand.game_type==BROWSER){
      switch(gameCommand.command){
        case START:
          if(this->game_status==STOPPED || this->game_status==PAUSED){
            this->game_status=ACTIVE;
            if(previousName!=this->game_name && previousStatus==PAUSED){
              // If I have another game paused, I must closed it before I start a new one
              this->closeBrowser();
              previousStatus=STOPPED;
            }
            if(previousStatus!=PAUSED){
              // If PAUSED it is not necessary to start again
              this->speakIntro(package);
              this->sendGameCommand(this->game_name);
              this->openGameInBrowser(); 
            }            
          }
          break;
        case STOP:
            this->game_status=STOPPED;
            this->closeBrowser();
          break;
        case PAUSE:
            this->game_status=PAUSED;
          break;
      }
  }
}
void alzBrowserGameClass::playGame(int argc, char** argv,string game_name,string package){
  //cout << "BROWSER -- PLAYGAME\n";

    this->previousName=this->game_name;
    this->game_name=game_name;
    this->previousStatus=this->game_status;
    this->numCards=8;
    this->package=package;
    ros::init(argc, argv, "alz_game_browser_node");
    ros::NodeHandle n;
    etts_say_text = n.advertise<etts_msgs::Utterance>("etts", 100);
    make_gesture = n.advertise<std_msgs::String>("keyframe_gesture_filename", 100);
    ask4GameCommand_memGame_pub = n.advertise<std_msgs::String>("ask4GameCommand", 1);
    internalFeedback_pub = n.advertise<memory_game_handler::Feedback>("GAME_INTERNAL_FEEDBACK",100);
    reportCommandResult_sub = n.subscribe("reportCommandResult", 1, &alzBrowserGameClass::dialogCallback_memGame, this);
    internalCommand_sub = n.subscribe("GAME_INTERNAL_COMMAND", 1, &alzBrowserGameClass::handleGame, this);
    
}
int alzBrowserGameClass::handleGameCommand(string command){
  if(this->game_status==ACTIVE){
    cout << "BROWSER -- HANDLE\n";
    cout <<this->game_name<<" *** Me has pedido que haga: " << command << "\n";
    if(command=="restart"){
      this->sendGameCommand(command);
    }else{
      if(this->game_name=="memory_game"){
        if(this->isANumber(command)){
          saySentence(getRandomNumberString(command),SHUTUP,0);
          this->sendGameCommand(command);
        }else if(command=="get_hint"){
          this->saySentence(getRandomString("Vale! Os doy una pista! | Venga, ahí va una pequeña ayuda!"),QUEUE,0);
          this->saySentence("Voy a marcar en verde dos cartas que forman una pareja!", QUEUE,0); 
          this->ask4Hint();
        }else{
          this->setLevelGame(command);
        }
      }else if (this->game_name=="differences_game"){
        if(isLevelCommand(command)){
          cout << "set level \n";
          this->setLevelGame(command);
        }else{
          this->sendGameCommand(command);
        }
      }
    }
  }
  return this->game_status;
}
