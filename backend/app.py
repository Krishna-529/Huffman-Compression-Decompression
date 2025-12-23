import os
import subprocess
from flask import Flask, request, send_file, jsonify
from flask_cors import CORS

app = Flask(__name__)
# Enable CORS for all routes (allows Netlify to talk to Render)
CORS(app)

UPLOAD_FOLDER = '/tmp/uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

# Detect Environment: If on Windows (dev), use .exe. If Linux (Render), use binary.
IS_WIN = os.name == 'nt'
COMPRESSOR = 'compressor.exe' if IS_WIN else './compressor'
DECOMPRESSOR = 'decompressor.exe' if IS_WIN else './decompressor'

@app.route('/health', methods=['GET'])
def health():
    return jsonify({"status": "active", "backend": "Flask + C++ Huffman"}), 200

@app.route('/process', methods=['POST'])
def process_file():
    if 'file' not in request.files:
        return jsonify({"error": "No file uploaded"}), 400
    
    file = request.files['file']
    action = request.form.get('action') # 'c' or 'd'
    
    if not file.filename:
        return jsonify({"error": "No filename"}), 400

    # Save input file
    input_path = os.path.join(UPLOAD_FOLDER, file.filename)
    file.save(input_path)
    
    # Determine Output Name and Executable
    if action == 'c':
        exe = COMPRESSOR
        output_name = f"{file.filename}.bin"
    else:
        exe = DECOMPRESSOR
        # Clean up name: remove .bin, prep for text
        clean_name = file.filename.replace(".bin", "")
        output_name = f"restored_{clean_name}.txt"

    output_path = os.path.join(UPLOAD_FOLDER, output_name)
    executable_path = os.path.join(os.getcwd(), exe)

    # Run C++
    try:
        # Command: ./compressor <input> <output>
        result = subprocess.run(
            [executable_path, input_path, output_path],
            capture_output=True, text=True, check=True
        )
    except subprocess.CalledProcessError as e:
        return jsonify({"error": "C++ Engine Failed", "details": e.stderr}), 500

    # Return processed file
    return send_file(output_path, as_attachment=True)

if __name__ == '__main__':
    port = int(os.environ.get("PORT", 5000))
    app.run(host='0.0.0.0', port=port)