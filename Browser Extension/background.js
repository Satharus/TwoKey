var activeUrl='';
var retrievedCredentials = "";
chrome.tabs.onUpdated.addListener(getTab)
function getTab()
{
        chrome.tabs.query({active: true, currentWindow: true}, tabs => {
                let link = tabs[0].url;
                 activeUrl=tabs[0].url;
                console.log(link);
                 var http=new XMLHttpRequest();
                 const url="http://localhost:8000/?url="+link;
                 http.open("GET",url,true);
                 http.send("");
                 http.onreadystatechange = (e) => {
				 retrievedCredentials = http.responseText;
				 if (retrievedCredentials.length != 0)
					console.log(retrievedCredentials);
                 }
				 var credentials = JSON.parse(retrievedCredentials);
				 console.log(credentials)
				 console.log(credentials[0].email + "  " + credentials[0].password);
				 chrome.tabs.sendMessage(tabs[0].id, credentials);
            });
}
chrome.tabs.onActivated.addListener(getTab)
function getTab()
{
        chrome.tabs.query({active: true, currentWindow: true}, tabs => {
                let link = tabs[0].url;
                 activeUrl=tabs[0].url;
                console.log(link);
                 var http=new XMLHttpRequest();
                 const url="http://localhost:8000/?url="+link;
                 http.open("GET",url,true);
                 http.send("");
                 http.onreadystatechange = (e) => {
				 retrievedCredentials = http.responseText;
				 if (retrievedCredentials.length != 0)
                                 
					console.log(retrievedCredentials);
                                        
                 }
				 var credentials = JSON.parse(retrievedCredentials);
				 console.log(credentials)
				 console.log(credentials[0].email + "  " + credentials[0].password);
				 chrome.tabs.sendMessage(tabs[0].id, credentials);
            });
}




