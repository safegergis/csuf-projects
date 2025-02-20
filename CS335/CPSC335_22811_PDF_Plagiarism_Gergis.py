"""
PDF Plagiarism Detector
Author: Mina Gergis
CPSC 335-22811

This program detects potential plagiarism between PDF documents using the KMP string matching algorithm.

Usage:
1. Run the program from command line with two PDF files as arguments:
   python CPSC335_22811_PDF_Plagiarism_Gergis.py master.pdf check.pdf
   
   Where:
   - master.pdf is the original document to check against
   - check.pdf is the document to check for potential plagiarism


The program uses the Knuth-Morris-Pratt (KMP) algorithm for efficient string pattern matching
to detect similar text sequences between the documents.
"""


import pymupdf
import argparse

def extract_text_from_pdf(pdf_path):
    text = ""
    try:
        doc = pymupdf.open(pdf_path)
        for page in doc:
            text += page.get_text()
        doc.close()
    except Exception as e:
        print(f"Error reading PDF file {pdf_path}: {str(e)}")
        return ""
    return text

def compute_kmp_table(pattern):
    table = [0] * len(pattern)
    j = 0
    for i in range(1, len(pattern)):
        while j > 0 and pattern[i] != pattern[j]:
            j = table[j-1]
        if pattern[i] == pattern[j]:
            j += 1
        table[i] = j
    return table

def kmp_search(text, pattern):
    if not pattern or not text:
        return 0
        
    matches = 0
    table = compute_kmp_table(pattern)
    j = 0
    
    for i in range(len(text)):
        while j > 0 and text[i] != pattern[j]:
            j = table[j-1]
        if text[i] == pattern[j]:
            j += 1
        if j == len(pattern):
            matches += 1
            j = table[j-1]
            
    return matches

def calculate_plagiarism(master_text, pattern_text):
    master_words = [word.lower() for word in master_text.split()]
    pattern_words = [word.lower() for word in pattern_text.split()]
    
    if not pattern_words:
        return 0
    
    master_str = ' '.join(master_words)
    pattern_str = ' '.join(pattern_words)
    
    total_matches = 0
    for i in range(len(pattern_words)):
        for j in range(i + 3, len(pattern_words) + 1):
            phrase = ' '.join(pattern_words[i:j])
            matches = kmp_search(master_str, phrase)
            if matches > 0:
                total_matches += len(phrase.split())
                
    plagiarism_percentage = (total_matches / len(pattern_words)) * 100
    return min(plagiarism_percentage, 100)

def main():
    parser = argparse.ArgumentParser(description='Check plagiarism between two PDF files')
    parser.add_argument('master_file', help='Path to the master PDF file')
    parser.add_argument('pattern_file', help='Path to the pattern PDF file')
    args = parser.parse_args()
    
    master_text = extract_text_from_pdf(args.master_file)
    pattern_text = extract_text_from_pdf(args.pattern_file)
    
    if not master_text or not pattern_text:
        print("Error: Could not process one or both PDF files.")
        return
        
    plagiarism_percent = calculate_plagiarism(master_text, pattern_text)
    
    likelihood = "High" if plagiarism_percent > 50 else "Low"
    
    print("\nPlagiarism Analysis Results:")
    print(f"Plagiarism Percentage: {plagiarism_percent:.2f}%")
    print(f"Plagiarism Likelihood: {likelihood}")

if __name__ == "__main__":
    main()
