use std::slice;
use std::os::raw::c_char;
use std::ffi::{CString, CStr};

use tokenizers::tokenizer::{Tokenizer};

const MODEL: &str = "bigscience/bloomz-560m";


fn _encode(text: &str) -> Vec<u32> {
    let t = Tokenizer::from_pretrained(MODEL, None).unwrap();
    match t.encode(text, false) {
        Ok(en) => {
            return en.get_ids().to_owned();
        },
        Err(_e) => {            
            return Vec::new()
        }
    };
}

fn _decode(ids: Vec<u32>) -> String {
    let t = Tokenizer::from_pretrained(MODEL, None).unwrap();
    match t.decode(ids, true) {
        Ok(st) => {            
            return st            
        },
        Err(_e) => {
            // error
            return String::from("");
        }
    };
}

#[no_mangle]
pub extern "C" fn decode(ids: *const u32, len: usize) -> *const c_char {
    let ids = unsafe { slice::from_raw_parts(ids, len) };    
    let text = _decode(ids.to_vec());
    CString::new(text).unwrap().into_raw()
}

#[no_mangle]
pub extern "C" fn encode(text: *const c_char) ->  *const u32 {    
    let text = unsafe { CStr::from_ptr(text).to_string_lossy().into_owned() };    
    let ids = _encode(text.as_str());    
    let arr = ids.as_ptr();
    std::mem::forget(ids);
    arr    
}

