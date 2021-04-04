function postToggle(toggle, connection, id)
{
    var http = new XMLHttpRequest();
    var data = new FormData();
    data.append('connection',connection);
    data.append('controlType','toggle');
    data.append('controlId',id);
    http.open('POST', 'action', true);
    http.send(data);
}

function postButton(connection,button)
{
    var http = new XMLHttpRequest();
    var data = new FormData();
    data.append('connection',connection);
    data.append('controlType','button');
    data.append('controlId',button);
    http.open('POST', 'action', true);
    http.send(data);
}

var updateURI = "update";

function forceUpdateUI()
{
    updateURI = "update-force";
    updateUI();
    updateURI = "update";
}    

function updateUI()
{
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() 
    {
        if (this.readyState == 4)
        {
            if(this.status == 200)
            {
                if(xhttp.responseText == '')
                {
                    console.log('no update');
                } 
                else 
                {
                    console.log('updating UI');
                    document.getElementById('demo').innerHTML = xhttp.responseText;
                }
            }
        } 
        
    };  
    xhttp.open('POST', updateURI, true);
    xhttp.send();
}
forceUpdateUI();
var intervalId = setInterval(function() {updateUI();}, 5000);