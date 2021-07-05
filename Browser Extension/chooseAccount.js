var i;
var creds_list;
chrome.runtime.onMessage.addListener(saveCreds);
function saveCreds(request,sender,sendResponse)
{
    creds_list=JSON.parse(request);
    alert(creds_list);
}
var x =["123@gmail.com","321@gmai.com","eee@gmail.com"];
for( i=0;i<creds_list.length();i++)
{
    chrome.contextMenus.create({id:creds_list[i].toString(),type:'radio', title:creds_list[i].email});
        
}


    chrome.contextMenus.onClicked.addListener((info, tab) => {
        // alert("Item " + info.menuItemId  + " clicked " +
        //             "in tab " + tab.id);

        var Nemail=info.menuItemId;
        var pw;
        for(var j=0;j<creds_list.length();j++)
        {
            if(Nemail==creds_list[j].email)
            {
                 pw=creds_list[j].password;
            }
         
        }
        if(input.srcElement.type==="email" || input.srcElement.type==="text")
        {
            input.target.value=email;
        }
        if(input.srcElement.type==="pass" || input.srcElement.type==="password")
        {
            input.target.value=pw;
        }
       
      });





// chrome.runtime.onMessage.addListener(choose_acc);
// function choose_acc(request,sender,sendResponse)
// {`
//     var emails=JSON.parse(request);
//     console.log(emails);
// }

// chrome.contextMenus.onClicked.addListener(clickData);
// function clickData()
// {
//     if(clickData.menuItemId == "chooseAccount" && clickData.editable || clickData.wasChecked)
//     {
//         alert("lets go ");
//     }
// }
