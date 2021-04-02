# Detouring-Hook
How it works ?  
#### -This technique called Mid detoruing hook aka stoling bytes hook.    
#### -First we need to specify which address that our target function is write.    
#### -After that we need to calculate the length of the necessary bytes that need to be hijacked.    
#### -Calculate the jump back by summation of the hook address plus the hook length.  
#### -Change memory protection and make sure the hook length is above 5 bytes.    
#### -Set nop sleds for safety.    
#### -calculate the size of the jump.  
#### -Add the jmp op code to the first byte of the hooked address.    
#### -Add +1 to the address so you dont overwrite the jmp and set the size of the jmp.  
#### -Restore the memory page protection.  
 
### We have implemented the hack for Plants And Zombie game, with this technique we have bypassed ASLR, so the cheat will work each time the game restarted.
