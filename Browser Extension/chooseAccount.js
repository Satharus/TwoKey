var i;
var emails_list;
chrome.runtime.onMessage.addListener((retrievedCredentials, sender, sendResponse) => {
    emails_list=retrievedCredentials;
    return true
});
 var x =["123@gmail.com","321@gmai.com","eee@gmail.com"];
for( i=0;i<3;i++)
{
    chrome.contextMenus.create({id:i.toString(),type:'radio', title:x[i]});
    chrome.contextMenus.onClicked.addListener((info, tab) => {
        alert("Item " + info.menuItemId  + " clicked " +
                    "in tab " + tab.id);
      });
    
}


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
