console.log("Hello");
document.addEventListener("keydown", function (event) {
  console.log("Key pressed: " + event.key);
  if (event.ctrlKey && event.key.toLowerCase() == "m") {
    event.preventDefault();
    console.log("Show login screen");
    getLoginInformation();
  }
});
let LoginStatus = checkCookie("screenID");
console.log(LoginStatus);
LoginStatus = LoginStatus && checkCookie("screenPWD");
console.log(LoginStatus);

if (LoginStatus == true) {
  if (ImageDisplayLoop() == false);

  document.cookie = "screenID=" + "" + "; Secure; SameSite=Strict; Path=/;";
  document.cookie = "screenPWD=" + "" + "; Secure; SameSite=Strict; Path=/;";
  alert("ID or Password Wrong");
} else {
  getLoginInformation();
}

function getLoginInformation() {
  let screenID = prompt("Enter Screen ID");
  let screenPWD = prompt("Enter Password");
  document.cookie =
    "screenID=" + screenID + "; Secure; SameSite=Strict; Path=/;";
  document.cookie =
    "screenPWD=" + screenPWD + "; Secure; SameSite=Strict; Path=/;";
  ImageDisplayLoop();
}

// Some cookie functions taken from W3schools
function getCookie(cname) {
  let name = cname + "=";
  let ca = document.cookie.split(";");
  for (let i = 0; i < ca.length; i++) {
    let c = ca[i];
    while (c.charAt(0) == " ") {
      c = c.substring(1);
    }
    if (c.indexOf(name) == 0) {
      return c.substring(name.length, c.length);
    }
  }
  return "";
}
function compCookie(KEY, VALUE) {
  let keyval = getCookie(KEY);
  console.log(KEY, keyval);
  if (keyval != VALUE) {
    return false;
  } else {
    return true;
  }
}

function checkCookie(KEY) {
  let keyval = getCookie(KEY);
  console.log(KEY, keyval);
  if (keyval != "") {
    return true;
  } else {
    return false;
  }
}
let token = 0;
const createImageMetaData = (time = "", name = "") => ({
  time: String(time),
  name: String(name),
});
const GlobalImageMetaData = {
  images: [],
};

function ImageDisplayLoop() {
  let header = document.getElementById("main-header-id");
  let image = document.getElementById("main-image-id");
  header.textContent = "Authenticating Images";
  let auth_status = authenticateWithServer();
  if (auth_status == false) {
    header.textContent = "Login Wrong ctrl+m";
    return false;
  }

  fetchImageURLs();
}
async function authenticateWithServer() {
  const url = "http://localhost:8080/api/auth";
  const credentials = {
    screenID: getCookie("screenID"),
    password: getCookie("screenPWD"),
  };
  try {
    const response = await fetch(url, {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify(credentials),
    });
    if (!response.ok) {
      return false;
    }
    const data = await response.json();
    token = data.token;
  } catch (error) {
    return false;
  }
  return true;
}
async function fetchImageURLs() {
  const url = "http://localhost:8080/api/images";
  try {
    const response = await fetch(url, {
      method: "GET",
      headers: {
        Authorization: "Bearer ${token}",
        Accept: "application/json",
      },
    });
    if (response.status === 401) {
      document.cookie = "screenID=" + "" + "; Secure; SameSite=Strict; Path=/;";
      document.cookie =
        "screenPWD=" + "" + "; Secure; SameSite=Strict; Path=/;";
      return false;
    }
    if (!response.ok) {
      return false;
    }
    const rawData = await response.json();
    if (Array.isArray(rawData)) {
      // Clean and map the incoming data using your factory function
      GlobalImageMetaData.images = rawData.map((item) =>
        createImageMetaData(item.time, item.name),
      );

      console.log(
        "Global state successfully updated!",
        GlobalImageMetaData.images,
      );
    }
  } catch (error) {
    return false;
  }
}
