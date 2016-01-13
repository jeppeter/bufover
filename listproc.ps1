
$global:strarr=@()
$global:parameters=$args

function AddStrArr
{
    foreach ($p in $args[0])
    {
        $s = "processid {1} name {2} command {0}" -f $p.CommandLine , $p.handle ,$p.name
        if ($global:strarr -notcontains $s)
        {
            $global:strarr += $s
        }
    }
}

try {
    $i = 0
    while(1)
    {
        $process = Get-WmiObject Win32_Process

        AddStrArr $process
        Start-Sleep -milliseconds 100
        $i ++
        if (($i % 10) -eq 0){
            $s = "time {0}" -f $i
            write-host $s
        }
    }    
}

finally{

    if ($global:parameters.count -gt 0){
        $file = $global:parameters[0]
        "" > $file
        foreach ($s in $global:strarr)
        {
            $s >> $file
        }        
    }else{
        foreach ($s in $global:strarr)
        {

            write-host $s
        }
    }
}