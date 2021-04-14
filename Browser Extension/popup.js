(function()
{
	var btn1=document.createElement("AutoFill");
	btn1.innerHTML="Fill";
	var x = document.createElement("P");
	x.innerText="Welcome again";
	var btn=document.getElementById('LoginBtn');
	
	
	var loggedinChecker=new XMLHttpRequest();
	const loggedin="http://localhost:8000/?loggedin=1";
	loggedinChecker.open("GET",loggedin,true);
	loggedinChecker.send();
	loggedinChecker.onreadystatechange = (e) => {
		if (loggedinChecker.responseText == "yes")
		{
			document.body.innerHTML="TwoKey";
			document.body.appendChild(x);
			document.body.appendChild(btn1);
			return;
		}
	console.log(loggedinChecker.responseText);
	}
	

	btn.addEventListener('click',f);
	function f()
	{
		var masterpasswd=document.getElementById('masterpasswd').value;
		var email=document.getElementById('email').value;
		
		var http=new XMLHttpRequest();
		const url="http://localhost:8000/?masterpasswd="+masterpasswd+"&email=" + email;
		http.open("GET",url,true);
		http.send();
		http.onreadystatechange = (e) => {
		console.log(http.readyState);
		if (http.readyState == 4 && http.status == 200) {
			document.body.innerHTML="TwoKey";
			document.body.appendChild(x);
			document.body.appendChild(btn1);
		}
		else if (http.readyState == 4)
		{
			alert("wrong password");
		}
		console.log(http.responseText);
	  }

	}
  chrome.runtime.onMessage.addListener(gotCreds);
  function gotCreds(credentials,sender,sendResponse)
  {
    btn1.addEventListener('click',autoFill);
    function autoFill()
    {
      chrome.runtime.sendMessage(credentials);
    }
  }


})();