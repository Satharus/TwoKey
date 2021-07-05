var i;
var emails_list = [];
chrome.runtime.onMessage.addListener(saveCreds);
function saveCreds(request, sender, sendResponse)
{
	console.log(request);
    emails_list=request;
	console.log(emails_list);
	var Nemail;
	var pw;
	for( i=0;i<emails_list.length;i++)
	{
	console.log("email list: ");
	console.log(emails_list)
    chrome.contextMenus.create({id:i.toString(),type:'radio', title:emails_list[i].email});
				alert(1);
    chrome.contextMenus.onClicked.addListener((info, tab) => {
		
		 Nemail=emails_list[info.menuItemId];
		
        for(var j=0;j<emails_list.length;j++)
        {
			alert(1);
            if(Nemail==emails_list[j].email)
            {
                 pw=emails_list[j].password;
            }
        } 
      });
	}

	
	document.addEventListener("click",fill_Creds);
		function fill_Creds(input)
		{
			alert(1);
			console.log(input);
			if(input.srcElement.type==="email" || input.srcElement.type==="text")
			{
				input.target.value=email;
			}
			if(input.srcElement.type==="pass" || input.srcElement.type==="password")
			{
				input.target.value=pw;
			}
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
