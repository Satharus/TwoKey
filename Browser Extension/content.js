document.addEventListener("input",get_cred);
function get_cred(input)
{
    if(input.type="email")
    var newEmail=input.target.value;
    if(input.type="password")
    var newPassword=input;
	var http=new XMLHttpRequest();
	const SavePwd="http://localhost:8000/?newPassword="+newPassword+ "&newEmail=" + newEmail +"&new="+ 1 ;
	http.open("GET",SavePwd,true);
	http.send();
	http.onreadystatechange = (e) => {

		alert(http.response);
	}
}

chrome.runtime.onMessage.addListener(fillCreds);
function fillCreds(request,sender,sendResponse)
{
	chrome.runtime.sendMessage(request);
	var credentials = JSON.parse(request);
	console.log(credentials);
	console.log(credentials[0].email + "  " + credentials[0].password);

  document.addEventListener("click",fill_Creds);
function fill_Creds(input)
{
	console.log(input);
    if(input.srcElement.type==="email" || input.srcElement.type==="text")
	{
		input.target.value=credentials[0].email;
	}
    if(input.srcElement.type==="pass" || input.srcElement.type==="password")
	{
		input.target.value=credentials[0].password;
	}
}
}
