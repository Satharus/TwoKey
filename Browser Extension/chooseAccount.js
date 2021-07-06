var i;
var emails_list = [];
chrome.runtime.onMessage.addListener(saveCreds);
function saveCreds(request, sender, sendResponse)
{
	//console.log(request);
    //emails_list=JSON.parse(request);
	//console.log(emails_list);
	emails_list=request;
	var Nemail;
	var pw;
	for( i=0;i<emails_list.length;i++)
	{
		// console.log("email list: ");
		// console.log(emails_list)
    	chrome.contextMenus.create({id:emails_list[i],type:'radio', title:emails_list[i]});
				
    
	}
	// chrome.contextMenus.onClicked.addListener((info, tab) => {
	// 	if(info.menuItemId=="les go")
	// 	{
	// 		console.log(1);
	// 		console.log(info.menuItemId);

	// 	}
	// 			Nemail=emails_list[info.menuItemId].email;
	// 	pw = emails_list[info.menuItemId].password;	
		
	// 	console.log(Nemail + pw);
    //   });
	
	chrome.contextMenus.onClicked.addListener(clickData);
	function clickData(info, tab)
	{
		console.log(6);
		console.log(info.menuItemId);
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


