import { useState, useCallback } from "react";
import { useDropzone } from "react-dropzone";
import { Card, CardContent, CardHeader, CardTitle, CardDescription } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { Progress } from "@/components/ui/progress";
import { Upload, FileArchive, Download, Loader2, CheckCircle2 } from "lucide-react";
import { toast } from "@/hooks/use-toast";
import FileStats from "./FileStats";

const API_URL = import.meta.env.VITE_API_URL || "http://localhost:5000";

const DecompressionPanel = () => {
  const [file, setFile] = useState<File | null>(null);
  const [isProcessing, setIsProcessing] = useState(false);
  const [result, setResult] = useState<{
    blob: Blob;
    filename: string;
    compressedSize: number;
    restoredSize: number;
  } | null>(null);

  const onDrop = useCallback((acceptedFiles: File[]) => {
    if (acceptedFiles.length > 0) {
      setFile(acceptedFiles[0]);
      setResult(null);
    }
  }, []);

  const { getRootProps, getInputProps, isDragActive } = useDropzone({
    onDrop,
    multiple: false,
    accept: {
      "application/octet-stream": [".bin"],
    },
  });

  const handleDecompress = async () => {
    if (!file) return;

    setIsProcessing(true);
    const formData = new FormData();
    formData.append("file", file);
    formData.append("action", "d");

    try {
      const response = await fetch(`${API_URL}/process`, {
        method: "POST",
        body: formData,
      });

      if (!response.ok) {
        const error = await response.json();
        throw new Error(error.error || "Decompression failed");
      }

      const blob = await response.blob();
      const cleanName = file.name.replace(".bin", "");
      const restoredFilename = `restored_${cleanName}.txt`;

      setResult({
        blob,
        filename: restoredFilename,
        compressedSize: file.size,
        restoredSize: blob.size,
      });

      toast({
        title: "Decompression Complete",
        description: `File restored successfully!`,
      });
    } catch (error) {
      toast({
        title: "Decompression Failed",
        description: error instanceof Error ? error.message : "An error occurred",
        variant: "destructive",
      });
    } finally {
      setIsProcessing(false);
    }
  };

  const handleDownload = () => {
    if (!result) return;

    const url = URL.createObjectURL(result.blob);
    const a = document.createElement("a");
    a.href = url;
    a.download = result.filename;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);
  };

  const handleReset = () => {
    setFile(null);
    setResult(null);
  };

  return (
    <Card className="border-border/50 shadow-lg">
      <CardHeader>
        <CardTitle className="flex items-center gap-2">
          <FileArchive className="w-5 h-5 text-primary" />
          Decompress a File
        </CardTitle>
        <CardDescription>
          Upload a .bin compressed file to restore the original content
        </CardDescription>
      </CardHeader>
      <CardContent className="space-y-6">
        {/* Dropzone */}
        <div
          {...getRootProps()}
          className={`
            border-2 border-dashed rounded-xl p-8 text-center cursor-pointer
            transition-all duration-200
            ${isDragActive 
              ? "border-primary bg-primary/5" 
              : "border-border hover:border-primary/50 hover:bg-muted/50"
            }
            ${file ? "border-primary/50 bg-primary/5" : ""}
          `}
        >
          <input {...getInputProps()} />
          {file ? (
            <div className="flex flex-col items-center gap-3">
              <div className="w-16 h-16 rounded-full bg-primary/10 flex items-center justify-center">
                <FileArchive className="w-8 h-8 text-primary" />
              </div>
              <div>
                <p className="font-medium text-foreground">{file.name}</p>
                <p className="text-sm text-muted-foreground">
                  {(file.size / 1024).toFixed(2)} KB
                </p>
              </div>
              <Button variant="ghost" size="sm" onClick={(e) => { e.stopPropagation(); handleReset(); }}>
                Choose different file
              </Button>
            </div>
          ) : (
            <div className="flex flex-col items-center gap-3">
              <div className="w-16 h-16 rounded-full bg-muted flex items-center justify-center">
                <Upload className="w-8 h-8 text-muted-foreground" />
              </div>
              <div>
                <p className="font-medium text-foreground">
                  {isDragActive ? "Drop the file here" : "Drag & drop a .bin file here"}
                </p>
                <p className="text-sm text-muted-foreground">
                  or click to select a compressed file
                </p>
              </div>
            </div>
          )}
        </div>

        {/* Decompress Button */}
        {file && !result && (
          <Button 
            onClick={handleDecompress} 
            className="w-full py-6 text-lg"
            disabled={isProcessing}
          >
            {isProcessing ? (
              <>
                <Loader2 className="w-5 h-5 mr-2 animate-spin" />
                Decompressing...
              </>
            ) : (
              <>
                <FileArchive className="w-5 h-5 mr-2" />
                Decompress File
              </>
            )}
          </Button>
        )}

        {/* Processing Indicator */}
        {isProcessing && (
          <div className="space-y-2">
            <Progress value={undefined} className="h-2" />
            <p className="text-sm text-center text-muted-foreground">
              Decoding Huffman compressed data...
            </p>
          </div>
        )}

        {/* Result */}
        {result && (
          <div className="space-y-4">
            <div className="flex items-center gap-2 text-green-600 dark:text-green-400">
              <CheckCircle2 className="w-5 h-5" />
              <span className="font-medium">Decompression Successful!</span>
            </div>

            <FileStats
              originalSize={result.compressedSize}
              processedSize={result.restoredSize}
              type="decompression"
            />

            <div className="flex gap-3">
              <Button onClick={handleDownload} className="flex-1 py-6">
                <Download className="w-5 h-5 mr-2" />
                Download Restored File
              </Button>
              <Button variant="outline" onClick={handleReset} className="py-6">
                Decompress Another
              </Button>
            </div>
          </div>
        )}
      </CardContent>
    </Card>
  );
};

export default DecompressionPanel;
