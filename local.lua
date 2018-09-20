

function update_configure(c)

	local x = c:add_maps()
	
	local l
	l = x:add_locals()
	l:set_addr("0.0.0.0")
	l:set_port(1935)
	

	
	local r
	r = x:add_remotes()

	r:set_addr("baidu.com")
    --r:set_addr("127.0.0.1")
	r:set_port(443)



    c:set_run_as_daemon(0)                                                          --服务器后台运行 using both
end
