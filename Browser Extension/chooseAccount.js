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
    chrome.contextMenus.create({id:creds_list[i].toString(),type:'radio', title:creds_list[i].email});

    chrome.contextMenus.onClicked.addListener((info, tab) => {
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
