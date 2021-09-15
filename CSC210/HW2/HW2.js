function ChmodHandler(){
   var owner=document.getElementsByName("Owner");
   var group=document.getElementsByName("Group");
   var public=document.getElementsByName("Public");
   var final = 0;
   var final_str = "";
   var str = "";
   for(var i=0;i<owner.length;i++){
        if(owner[i].checked){
        	final += parseInt(owner[i].value,10);
        	if(owner[i].value==="400")
        	{
        		str+="r"
        	}
        	else if(owner[i].value==="200")
        	{
        		str+="w"
        	}
        	else if(owner[i].value==="100")
        	{
        		str+="x"
        	}
        }else{
        	str+="-"
        }	
   }
   for(var j=0;j<group.length;j++){
        if(group[j].checked){
        	final += parseInt(group[j].value,10);
        	if(group[j].value==="040")
        	{
        		str+="r"
        	}
        	else if(group[j].value==="020")
        	{
        		str+="w"
        	}
        	else if(group[j].value==="010")
        	{
        		str+="x"
        	}
        }else{
        	str+="-"
        }	
   }
   for(var k=0;k<public.length;k++){
        if(public[k].checked){
        	final += parseInt(public[k].value,10);
        	if(public[k].value==="004")
        	{
        		str+="r"
        	}
        	else if(public[k].value==="002")
        	{
        		str+="w"
        	}
        	else if(public[k].value==="001")
        	{
        		str+="x"
        	}
        }else{
        	str+="-"
        }
   }
   if (final < 10){
   	final_str = "00" + final.toString()
   } else if (final < 100){
   	final_str = "0" + final.toString()
   } else {
   	final_str = final.toString()
   }
   document.getElementById("final").value = final_str;
   document.getElementById("str").value = str;   
}