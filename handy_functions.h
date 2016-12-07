uint8_t explode(String *results, String source, char delimiter)
{
    uint8_t count = 0;
    uint8_t index = 0;
    
    //results[count++] = source;
    
    for (uint8_t i = 0; i < source.length(); i++) {
        if (source.charAt(i) == delimiter) {
            results[count++] = source.substring(index, i);
            index = i+1;
        }
    }
    results[count++] = source.substring(index);

    return count;
}