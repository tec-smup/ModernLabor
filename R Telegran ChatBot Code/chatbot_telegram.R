#criar token reiniciar R
user_renviron = path.expand(file.path("~", ".Renviron"))
if(!file.exists(user_renviron)) # check to see if the file already exists
  file.create(user_renviron)
file.edit(user_renviron) # open with another text editor if this fails

install.packages("telegram")
install.packages("googlesheets")
Sys.setenv(TZ = "America/Sao_Paulo")
Sys.timezone(location = TRUE)


library("plyr")
library("jsonlite")
library("telegram")
library("googlesheets")
suppressMessages(library(dplyr))
library(ggplot2)



bot <- TGBot$new(token=bot_token("StartmeupBot"))
bot$getMe()
UltimaMensagem =0
mensagematual = 0
UltimaMensagem1=0
mensagematual1=0


while(TRUE){
  
  
  msgs<-bot$getUpdates()
  length(msgs)
  if (length(msgs)>0){
    
    resposta=""
    chats<-msgs
    chats<-chats[chats$message$message_id==max(chats$message$message_id),TRUE]
    
    UltimaMensagem1<-chats$message$message_id
    if(UltimaMensagem1>mensagematual1){
      
      bot$set_default_chat_id(348024465)
      m<-paste(chats$message$from$first_name,chats$message$text,chats$message$from)
      bot$sendMessage(m)

      chats<-chats[chats$message$from==348024465,TRUE] 
      if (nrow(chats)>0){
        chats<-na.exclude(chats)
        resposta<-substr(chats$message$text,1,8) 
        if (resposta=="RESPONDA"){
          ID_DESTINO <-substr(chats$message$text,10,19)
          MENSAGEM<-substr(chats$message$text,20,500)
          testa<-substr(chats$message$text,19,19)
          if(testa=="-"){
            ID_DESTINO<-as.numeric(substr(chats$message$text,10,18)) 
          }
          if(testa!="-"){
            ID_DESTINO <-substr(chats$message$text,10,19)
            ID_DESTINO<-as.numeric(ID_DESTINO)
          }
          bot$set_default_chat_id(ID_DESTINO)
          
          bot$sendMessage(MENSAGEM)
          
        }
        
      }
      
      
      mensagematual1<-UltimaMensagem1
    }
    
    
    
    chats<-msgs[msgs$message$text=="/prod",TRUE]
    nome<-paste(chats$message$from$first_name,chats$message$from$last_name)
   
    nome <-  gsub(" ","%20",nome)
    
    bot$set_default_chat_id(chats$message$from$id)
    if (nrow(chats)>0){
      
      
      chat2<-msgs[msgs$message$from$id==348024465,TRUE] 
   
        bot$sendMessage("Informe quantidade produzida KG")
        bot$getUpdates(offset = msgs$update_id +1) 
        
        while(TRUE){
          msgs<-bot$getUpdates()
          chats<-msgs
          if (length(msgs)>0){
            break
          }
        }
        chats<-chats[chats$message$message_id==max(chats$message$message_id),TRUE]
        quant<-chats$message$text
        if (nrow(chats)>0){
          
          
          chat2<-msgs[msgs$message$from$id==348024465,TRUE] 
          
          bot$sendMessage("Observações")
          bot$getUpdates(offset = msgs$update_id +1) 
          
          while(TRUE){
            msgs<-bot$getUpdates()
            chats<-msgs
            if (length(msgs)>0){
              break
            }
          }
          chats<-chats[chats$message$message_id==max(chats$message$message_id),TRUE]
          obs<-chats$message$text
          obs <-  gsub(" ","%20",obs)
        
        url <-paste0('https://oee-server.appspot.com/api/feed/update?token=67RRJQRANOMPQ30Q&mc_cd=OP1&field1=',nome,'&field3=',quant,'&field5=',obs)
        
       res <- fromJSON(txt = url )
        m<-paste0("Sucesso ",quant, " Kg lançado verifique Dash" )
        bot$sendMessage(m)
        
        
      } # fim if2
    

    }# fim if1
  }

    
  bot$getUpdates(offset = msgs$update_id +1)
}


