import { ArrowRight, TrendingDown, TrendingUp } from "lucide-react";

interface FileStatsProps {
  originalSize: number;
  processedSize: number;
  type: "compression" | "decompression";
}

const formatSize = (bytes: number): string => {
  if (bytes < 1024) return `${bytes} B`;
  if (bytes < 1024 * 1024) return `${(bytes / 1024).toFixed(2)} KB`;
  return `${(bytes / (1024 * 1024)).toFixed(2)} MB`;
};

const FileStats = ({ originalSize, processedSize, type }: FileStatsProps) => {
  const ratio = type === "compression" 
    ? ((1 - processedSize / originalSize) * 100)
    : ((processedSize / originalSize - 1) * 100);

  const isCompression = type === "compression";

  return (
    <div className="bg-muted/50 rounded-xl p-6 space-y-4">
      <div className="flex items-center justify-between gap-4">
        {/* Original Size */}
        <div className="flex-1 text-center">
          <p className="text-sm text-muted-foreground mb-1">
            {isCompression ? "Original" : "Compressed"}
          </p>
          <p className="text-2xl font-bold text-foreground">
            {formatSize(originalSize)}
          </p>
        </div>

        {/* Arrow */}
        <div className="flex-shrink-0">
          <div className="w-10 h-10 rounded-full bg-primary/10 flex items-center justify-center">
            <ArrowRight className="w-5 h-5 text-primary" />
          </div>
        </div>

        {/* Processed Size */}
        <div className="flex-1 text-center">
          <p className="text-sm text-muted-foreground mb-1">
            {isCompression ? "Compressed" : "Restored"}
          </p>
          <p className="text-2xl font-bold text-foreground">
            {formatSize(processedSize)}
          </p>
        </div>
      </div>

      {/* Ratio */}
      <div className="flex items-center justify-center gap-2 pt-2 border-t border-border">
        {isCompression ? (
          <>
            <TrendingDown className="w-5 h-5 text-green-600 dark:text-green-400" />
            <span className="text-lg font-semibold text-green-600 dark:text-green-400">
              {ratio.toFixed(1)}% size reduction
            </span>
          </>
        ) : (
          <>
            <TrendingUp className="w-5 h-5 text-blue-600 dark:text-blue-400" />
            <span className="text-lg font-semibold text-blue-600 dark:text-blue-400">
              {ratio.toFixed(1)}% data restored
            </span>
          </>
        )}
      </div>
    </div>
  );
};

export default FileStats;
