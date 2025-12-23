import { useState } from "react";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { FileArchive, FileOutput } from "lucide-react";
import CompressionPanel from "@/components/CompressionPanel";
import DecompressionPanel from "@/components/DecompressionPanel";

const Index = () => {
  return (
    <div className="min-h-screen bg-background">
      <div className="container mx-auto px-4 py-12 max-w-4xl">
        {/* Header */}
        <div className="text-center mb-12">
          <h1 className="text-4xl md:text-5xl font-bold text-foreground mb-4 tracking-tight">
            Huffman Compression
          </h1>
          <p className="text-muted-foreground text-lg max-w-2xl mx-auto">
            Efficient lossless data compression using Huffman coding algorithm.
            Compress your files or decompress previously encoded data.
          </p>
        </div>

        {/* Main Tabs */}
        <Tabs defaultValue="compress" className="w-full">
          <TabsList className="grid w-full grid-cols-2 mb-8">
            <TabsTrigger value="compress" className="flex items-center gap-2 text-base py-3">
              <FileArchive className="w-5 h-5" />
              Compress
            </TabsTrigger>
            <TabsTrigger value="decompress" className="flex items-center gap-2 text-base py-3">
              <FileOutput className="w-5 h-5" />
              Decompress
            </TabsTrigger>
          </TabsList>

          <TabsContent value="compress">
            <CompressionPanel />
          </TabsContent>

          <TabsContent value="decompress">
            <DecompressionPanel />
          </TabsContent>
        </Tabs>

        {/* Footer */}
        <div className="mt-16 text-center text-muted-foreground text-sm">
          <p>Powered by C++ Huffman Engine</p>
        </div>
      </div>
    </div>
  );
};

export default Index;
