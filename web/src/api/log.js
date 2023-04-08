import { service } from '@/utils/request'

// 日志信息接口
export const LogInfo = () => {
    return service({
        url: '/log/info',
        method: 'get'
    })
}

// 删除日志
export const LogDelete = (logIds) => {
    const url = 'log/delete?log_ids=' + logIds
    return service({
        url: url,
        method: 'delete'
    })
}

