var confirmType='';
function ChkNumberRange(inValue, minValue, maxValue){
	if (inValue=='' || isNaN(inValue) || (inValue < minValue) || (inValue > maxValue)) return false;
	else return true;
}
function ChkIsHexCode(inValue, inLen){
	var i;
	for (i = 0; i < inLen; i ++){
		if (!(inValue.charAt(i) >= '0' && inValue.charAt(i) <= '9' || 
			  inValue.charAt(i) >= 'a' && inValue.charAt(i) <= 'f' || 
			  inValue.charAt(i) >= 'A' && inValue.charAt(i) <= 'F'))
			return false;
	}
	return true;
}
function ChkIp(s){
	var i;
	var split_s = s.split('.');
	var dotCnt = split_s.length;
	if (dotCnt != 4) return -1;
	for (i=0; i<dotCnt ; i++){
		if ((split_s[i].length > 3) || (split_s[i].length == 0)) return -1;
		if (isNaN(split_s[i]) || split_s[i] > 255 || split_s[i] < 0) return -1;
	}
	return split_s;
}
function ChkIpFormat(s){
	var i, j=0;
	for (i=0; i<s.length; i++){
		if (s.charAt(i) != '.' && isNaN(s.charAt(i)))
			return false;
	}
	return true;
}
function ChkEmail(s){
	var pattern = /^[\w\.\-]+@([\w\-]+\.)+[a-zA-Z]+$/;
	return pattern.test(s);
}
function chkForm(index) {
	var items = document.forms[index].elements, i, errFlag = 0, groupErrFlag = 0, typetmp;
	for (i = 0; i < items.length; i ++) {
		var errMsg = "";
		if (confirmType=='reboot' || confirmType=='restore') break;
		switch (items[i].name) {
		case "beacon_intv":
			if (ChkNumberRange(items[i].value, 20, 1000) == false){errMsg = " Invalid";}
			break;
		case "rts_threshold":
			if (ChkNumberRange(items[i].value, 0, 2432) == false){errMsg = " Invalid";}
			break;
		case "aes_passphrase":
			if (ChkNumberRange(items[i].value.length, 8, 63) == false){errMsg = " Invalid";}
			break;
		case "txtimer":
			if (ChkNumberRange(items[i].value, 10, 65535) == false){errMsg = " Invalid";}
			break;
		case "s_lstport":
		case "c_desport":
			if (ChkNumberRange(items[i].value, 1024, 65535) == false){errMsg = " Invalid";}
			break;
		case "static_ip":
		case "gateway_ip":
		case "tftps_ip_b":
		case "tftps_ip":
		case "tftps_ip_w":
		case "dhcps_startip":
		case "dhcps_endip":
		case "dhcps_gateway":
		case "dns_ip":
			var ip = ChkIp(items[i].value);
			if (ip == -1 || ip[3]==255 || ip[3]==0 || (ip[0]&0xF0)>=0xE0 || ((ip[0]&0xC0) && ip[2]==0xFF)){errMsg = " Invalid";}
			else if (items[i].name=="dhcps_endip"){
				var maskip = ChkIp(items[i+2].value), k;
				if (maskip == -1 || maskip[0]==0 || maskip[1]==0) break;
				var staip = ChkIp(items[i-1].value);
				for (k=0;k<4;k++){
					if ((maskip[k]&staip[k]) != (maskip[k]&ip[k])){break;}
				}
				if (k<4){errMsg = " Invalid IP pool (net ID error)";break;}
				for (k=0;k<4;k++){
					if ((~maskip[k]&staip[k]) < (~maskip[k]&ip[k])){break;}
				}
				if (k==4){errMsg = " Invalid IP pool (end pool must large than start pool)";break;}
			}
			break;
		case "mask":
		case "dhcps_mask":
			var ip = ChkIp(items[i].value);
			if (ip == -1 || ip[0]==0 || ip[1]==0){errMsg = " Invalid";}
			break;
		case "new_usn":
		case "old_psw":
		case "new_psw":
		case "cfm_psw":
		case "username":
		case "password":
		case "file_name_b":
		case "file_name":
		case "file_name_w":
		case "ssid":
		case "dsm_name":
			if (items[i].value == ""){errMsg = " Invalid";}
			break;
		case "key_index_0":
		case "key_index_1":
		case "key_index_2":
		case "key_index_3":
			if (document.getElementById("key_length").value == 0){if (items[i].value.length != 10) {errMsg = " Invalid";}}
			else {if (items[i].value.length != 26) {errMsg = " Invalid";}}
			if (ChkIsHexCode(items[i].value, items[i].value.length) == false) {errMsg = " Invalid";}
			break;
		case "c_deshn":
		case "email_addr":
			if (items[i].value == "") {errMsg = " Invalid";}
			else if (ChkIpFormat(items[i].value) == true){var ip = ChkIp(items[i].value);
			if (ip == -1 || ip[3]==255 || ip[3]==0 || (ip[0]&0xF0)>=0xE0 || ((ip[0]&0xC0) && ip[2]==0xFF)){errMsg = " Invalid";}}
			break;
		case "email_from":
		case "email_to1":
		case "email_to2":
		case "email_to3":
			if (items[i].value != "" && ChkEmail(items[i].value)==false) {errMsg = " Invalid";}
			break;
		case "acc_ip1":
		case "acc_ip2":
		case "acc_ip3":
		case "acc_ip4":
			if (items[i].value != ""){var ip = ChkIp(items[i].value);
			if (ip == -1 || ip[3]==255 || ip[3]==0 || (ip[0]&0xF0)>=0xE0 || ((ip[0]&0xC0) && ip[2]==0xFF)){errMsg = " Invalid";}}
			break;
		case "security_mode":
			if (items[i-3].selectedIndex==1 && (items[i].selectedIndex==3 || items[i].selectedIndex==4)){
				errMsg=" AES/TKIP is unsupported in Ad-hoc mode.";
			}
			break;
		case "stop":
			if (items[i-2].selectedIndex==0 && items[i].selectedIndex==2){
				errMsg=" 2 stop bits is unsupported in 5 data bits mode";
			}
			break;
		default:
			continue;	break;
	}
		var errSpan = document.createElement("span");
		errSpan.appendChild(document.createTextNode(errMsg));
		errSpan.className = "errMsg0";
		errParent = items[i].parentNode;
		if (errParent.lastChild.className == "errMsg0")
			errParent.replaceChild(errSpan, errParent.lastChild);
		else
			errParent.appendChild(errSpan);
		if (errMsg != "") errFlag = 1;
}
	if (errFlag) return false;
	typetmp=confirmType;
	confirmType='';
	switch (typetmp){
	case 'submit':
		if (!confirm("To submit configuration! continue?")) return false; break;
	case 'upgrade':
		if (!confirm("The device will perform firmware update mission, please to confirm your TFTP server IP and file name again.\r\npress ok to continue, otherwise press cancel to close this window.")) return false;break;
	case 'submitAndReboot':
		if (!confirm("To submit configuration and reboot device! continue?")) return false;break;
	case 'reboot':
		if (!confirm("To reboot device! continue?")) return false;break;
	case 'restore':
		if (!confirm("To restore device! continue?")) return false;break;
	}
	return true;
}


