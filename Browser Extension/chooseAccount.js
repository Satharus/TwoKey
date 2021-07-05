var i;
var emails_list = [];
chrome.runtime.onMessage.addListener(saveCreds);
function saveCreds(request, sender, sendResponse)
{
	console.log(request);
    emails_list=request;
	console.log(emails_list);
	
	for( i=0;i<emails_list.length;i++)
	{
	console.log("email list: ");
	console.log(emails_list)
    chrome.contextMenus.create({id:i.toString(),type:'radio', title:emails_list[i].email});
    chrome.contextMenus.onClicked.addListener((info, tab) => {
        alert("Item " + info.menuItemId  + " clicked " +
                    "in tab " + tab.id);
      });
    
	}
	return true;
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
