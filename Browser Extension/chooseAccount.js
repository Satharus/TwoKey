var i;
var t;
var emails_list = [];
var email_pw=[];
chrome.runtime.onMessage.addListener(saveCreds);
function saveCreds(request, sender, sendResponse)
{
	//console.log(request);
    //emails_list=JSON.parse(request);
	//console.log(emails_list);
	emails_list=JSON.parse(request);
	
	var Nemail;
	var pw;
	chrome.contextMenus.removeAll();
	for( i=0;i<emails_list.length;i++)
	{
		// console.log("email list: ");
		//console.log(emails_list)
    	chrome.contextMenus.create({id:emails_list[i].email,type:'radio', title:emails_list[i].email});
				
    
	}
	
	
	chrome.contextMenus.onClicked.addListener(clickData);
	function clickData(info, tab)
	{
		//console.log(6);
		//console.log(info.menuItemId);
		
		email_pw=[info.menuItemId,""];
		
		console.log(email_pw[0]+"	"+email_pw[1]);
		
		chrome.tabs.sendMessage(tab.id,email_pw);
		//chrome.tabs.sendMessage(tab.id,email_pw);
		
		//chrome.runtime.onMessage.addListener(sendCreds);
			
		//	console.log(input);
		//input.target.value=creds[0].email;
		//input.target.value=info.menuItemId;
		//input.target.value=creds[0].password;
		//input.target.value="123";
		
		
		
		//console.log(emails_list[info.menuItemId]);
		//Chrome.tabs.sendMessage(tab.id, JSON.stringify(emails_list[info.menuItemId]));
	}
			
	

}


// chrome.runtime.onMessage.addListener(choose_acc);
// function choose_acc(request,sender,sendResponse)
// {`
//     var emails=JSON.parse(request);
//     console.log(emails);
// }


